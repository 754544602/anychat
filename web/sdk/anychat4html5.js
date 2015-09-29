var AnyChatSDK = function() {
    var PeerConnection = (window.PeerConnection || window.webkitPeerConnection00 || window.webkitRTCPeerConnection || window.mozRTCPeerConnection);
    var URL = (window.URL || window.webkitURL || window.msURL || window.oURL);
    var getUserMedia = (navigator.getUserMedia || navigator.webkitGetUserMedia || navigator.mozGetUserMedia || navigator.msGetUserMedia);
    var nativeRTCIceCandidate = (window.mozRTCIceCandidate || window.RTCIceCandidate);
    var nativeRTCSessionDescription = (window.mozRTCSessionDescription || window.RTCSessionDescription); // order is very important: "RTCSessionDescription" defined in Nighly but useless
    var moz = !!navigator.mozGetUserMedia;
    var iceServer = {
        "iceServers": [{
            "url": "stun:stun.l.google.com:19302"
        }]
    };

    function EventEmitter() {
        this.events = {};
    }
    //绑定事件函数
    EventEmitter.prototype.on = function(eventName, callback) {
        this.events[eventName] = this.events[eventName] || [];
        this.events[eventName].push(callback);
    };
    //触发事件函数
    EventEmitter.prototype.emit = function(eventName, _) {
        var events = this.events[eventName],
            args = Array.prototype.slice.call(arguments, 1),
            i, m;

        if (!events) {
            return;
        }
        for (i = 0, m = events.length; i < m; i++) {
            events[i].apply(null, args);
        }
    };


    /**********************************************************/
    /*                                                        */
    /*                   流及信道建立部分                     */
    /*                                                        */
    /**********************************************************/


    /*******************基础部分*********************/
    function anychat() {		
		this.localStatus = {socketConnect:false, sdkauthpass:"",
							loginuser:"", loginpass:"", loginparam:0, 
							roomid:-1, roomname:"", roompass:"", roomparam:0,
							selfuserid:-1};
							
		// 连接服务器成功之前的参数设置数据
		this.sdkoptionData = {};
		// 用户基本信息
		this.userInfoData = {};
		// 用户好友信息
		this.friendInfoData = {};
		// 用户分组信息
		this.groupInfoData = {};
		// 业务对象数据
		this.objectInfoData = {};
		// 房间用户数组
		this.roomusers = [];
		// 队列等待时间计数器
		this.queueWaitSeconds = 0;
		this.queueWaitTimer = -1;
			
        //本地media stream
        this.localMediaStream = null;
		this.localVideoObject = null;
		this.remoteMediaStream = null;
		this.remoteVideoObject = null;
		
        //本地WebSocket连接
        this.socket = null;
        //本地socket的id，由后台服务器创建
        this.me = null;
        //保存所有与本地相连的peer connection， 键为socket id，值为PeerConnection类型
        this.peerConnections = {};
        //保存所有与本地连接的socket的id
        this.connections = [];
        //初始时需要构建链接的数目
        this.numStreams = 0;
        //初始时已经连接的数目
        this.initializedStreams = 0;
    }
    //继承自事件处理器，提供绑定事件和触发事件的功能
    anychat.prototype = new EventEmitter();


	function addlog(funcname, message) {
	console.warn(funcname);
	if(message.length)
		console.log("\t " + message);
	}
	
	function clearAllStatus(){
		clearRoomStatus();
		this.anychat.userInfoData = {};
		this.anychat.friendInfoData = {};
		this.anychat.groupInfoData = {};
		this.anychat.objectInfoData = {};
		this.anychat.localStatus.loginuser = "";
		this.anychat.localStatus.selfuserid = -1;
	}
	
	function clearRoomStatus(){
		this.anychat.roomusers = [];
		this.anychat.localStatus.roomid = -1;
		this.anychat.localStatus.roomname = "";
	}


    /*************************服务器连接部分***************************/
	
	anychat.prototype.InitSDK = function(dwFlags) {
		var that = this;
		this.on('_notifyMessage', function(data) {
			var ls = that.localStatus;
			if(data.event == WM_GV_CONNECT) {
				if(data.lParam == 0 && ls.loginuser.length) {
					that.socket.sendCommand("login", {"username":ls.loginuser, "password":ls.loginpass, "param":ls.loginparam, "sdkauthpass":ls.sdkauthpass});
				}
			} else if(data.event == WM_GV_LOGINSYSTEM) {
				if(data.lParam == 0) {
					that.createStream();
					that.localStatus.selfuserid = data.wParam;
					// 将没有同步的参数设置传给服务器
					for(var optname in that.sdkoptionData) {
						that.socket.sendCommand(that.sdkoptionData[optname].command, that.sdkoptionData[optname].param);
					}
					// 自动进入房间
					if(that.localStatus.roomid != -1) {
						that.socket.sendCommand("enterroom", {"roomid":ls.roomid, "password":ls.roompass, "param":ls.roomparam});
					} else if(that.localStatus.roomname.length) {
						that.socket.sendCommand("enterroomex", {"roomname":ls.roomname, "password":ls.roompass});
					}
				}
			} else if(data.event == WM_GV_ENTERROOM) {
				var userid = data.wParam;
				var bEnter = data.lParam;
				if(bEnter) {
					that.roomusers.push(userid);
				} else {
					for(var i=0; i<that.roomusers; i++) {
						if(that.roomusers[i] == userid) {
							delete that.roomusers[i];
							break;
						}
					}
				}
			}
			that.emit("OnNotifyMessage", data.event, data.wParam, data.lParam);
		});
		// 收到视频呼叫事件通知
		this.on('_videoCallEvent', function(data) {
			that.emit("OnVideoCallEvent", data.event, data.userid, data.errorcode, data.flags, data.param, data.userstr);
		});
		// 收到透明通道事件通知
		this.on('_transBufferEvent', function(data) {
			that.emit("OnTransBuffer", data.userid, data.buf, data.buf.length);
		});
		// 收到文字聊天事件通知
		this.on('_textMessageEvent', function(data) {
			that.emit("OnTextMessage", data.fromuserid, data.touserid, data.secret, data.msgbuf, data.msgbuf.length);
		});
		// 收到用户基础数据，保存到json对象中去
		this.on('_userBaseInfo', function(data) {
			var info = {"nickname":data.nickname, "level":data.level, "internetip":data.internetip};
			that.userInfoData[data.userid] = info
		});
		// 收到用户好友数据，保存到json对象中去
		this.on('_userFriendInfo', function(data) {
			var info = {"groupid":data.groupid, "status":data.status, "t1":data.t1, "t2":data.t2, "t3":data.t3, "t4":data.t4, "t5":data.t5, "t6":data.t6, "t7":data.t7, "t8":data.t8, "t9":data.t9, "t10":data.t10};
			that.friendInfoData[data.userid] = info
		});
		// 收到用户分组数据，保存到json对象中去
		this.on('_groupBaseInfo', function(data) {
			var info = {"groupname":data.groupname};
			that.groupInfoData[data.groupid] = info
		});		
		// 收到房间用户列表，更新本地数据
		this.on('_roomUserList', function(data) {
			that.roomusers = [];
			var useridlist = data.useridlist.split(";");
			for(i=0; i<useridlist.length; i++) {
				if(useridlist[i] != "")
					that.roomusers.push(useridlist[i]);
			}
		});
		// 收到业务对象数据，保存到json对象中去
		this.on('_objectBaseInfo', function(data) {
			var status = {};
			if(typeof that.objectInfoData[data.objecttype + "_" + data.objectid] != "undefined")
				status = that.objectInfoData[data.objecttype + "_" + data.objectid].status;
			var info = {"objecttype":data.objecttype, "objectid":data.objectid, "flags":data.flags, "objectname":data.objectname, "priority":data.priority, "attribute":data.attribute, "inttag":data.inttag, "objectdesc":data.objectdesc, "strtag":data.strtag, "status":status};
			that.objectInfoData[data.objecttype + "_" + data.objectid] = info;
		});
		// 收到业务对象状态信息，保存到json对象中去
		this.on('_objectStatus', function(data) {
			for(var i in that.objectInfoData) {
				if(that.objectInfoData[i].objecttype == data.objecttype && that.objectInfoData[i].objectid == data.objectid) {
					for(var k in data) {
						if(k == "objecttype" || k == "objectid")
							continue;
						that.objectInfoData[i].status[k] = data[k];
					}
					break;
				}
			}
		});
		// 收到业务对象异步事件通知
		this.on('_objectEvent', function(data) {
			if(data.event == ANYCHAT_QUEUE_EVENT_ENTERRESULT) {
				that.queueWaitSeconds = 0;
				if(that.queueWaitTimer == -1) {
					that.queueWaitTimer = setInterval(function () {
						that.queueWaitSeconds++;
					}, 1000);
				}
			} else if(data.event == ANYCHAT_QUEUE_EVENT_LEAVERESULT) {
				if(that.queueWaitTimer != -1)
					clearInterval(that.queueWaitTimer);
			}
			that.emit("OnObjectEvent", data.objecttype, data.objectid, data.event, data.param1, data.param2, data.param3, data.param4, data.strparam);
		});
		
	}


    //本地连接信道，信道为websocket
    anychat.prototype.Connect = function(server, port) {
        var socket, that = this;
		var webSocketPort = (parseInt(port) + 2);;
		var wsurl = "ws://" + server + ":" + webSocketPort;
		if (typeof MozWebSocket != "undefined") {
			socket = this.socket = new MozWebSocket(wsurl, "anychat-protocol");
		} else {
			socket = this.socket = new WebSocket(wsurl, "anychat-protocol");
		}
		
		socket.sendCommand = function(name, data) {
			socket.send(JSON.stringify({
                "eventName": name,
                "data": data
            }));
		}
		
        socket.onopen = function() {
			socket.sendCommand("connect", {"os":0, "host":server, "port":webSocketPort});
			that.emit("socket_opened", socket);
			that.localStatus.socketConnect = true;
        };

        socket.onmessage = function(message) {
			//addlog("socket.onmessage", message.data);
            var json = JSON.parse(message.data);
            if (json.eventName) {
                that.emit(json.eventName, json.data);
            } else {
                that.emit("socket_receive_message", socket, json);
            }
        };

        socket.onerror = function(error) {
			if(that.localStatus.socketConnect)
				that.emit("OnNotifyMessage", WM_GV_LINKCLOSE, 0, 101);
			else
				that.emit("OnNotifyMessage", WM_GV_CONNECT, 0, 115);
            that.emit("socket_error", error, socket);
			that.localStatus.socketConnect = false;
        };

        socket.onclose = function(data) {
/*          that.localMediaStream.close();
            var pcs = that.peerConnections;
            for (i = pcs.length; i--;) {
                that.closePeerConnection(pcs[i]);
            }
            that.peerConnections = [];
            that.dataChannels = {};
            that.fileChannels = {};
            that.connections = [];
            that.fileData = {};
            that.emit('socket_closed', socket);
*/			
			that.emit("OnNotifyMessage", WM_GV_LINKCLOSE, 0, 0);
			that.socket = null;
			that.localStatus.socketConnect = false;
			clearAllStatus();
        };

        this.on("_ice_candidate", function(data) {
            var candidate = new nativeRTCIceCandidate(data);
            var pc = that.peerConnections[data.userid];
            pc.addIceCandidate(candidate);
            that.emit('get_ice_candidate', candidate);
        });

        this.on('_new_peer', function(data) {
            that.connections.push(data.socketId);
            var pc = that.createPeerConnection(data.socketId),
                i, m;
            pc.addStream(that.localMediaStream);
            that.emit('new_peer', data.socketId);
        });

        this.on('_remove_peer', function(data) {
            var sendId;
            that.closePeerConnection(that.peerConnections[data.socketId]);
            delete that.peerConnections[data.socketId];
            delete that.dataChannels[data.socketId];
            for (sendId in that.fileChannels[data.socketId]) {
                that.emit("send_file_error", new Error("Connection has been closed"), data.socketId, sendId, that.fileChannels[data.socketId][sendId].file);
            }
            delete that.fileChannels[data.socketId];
            that.emit("remove_peer", data.socketId);
        });

        this.on('_offer', function(data) {
            that.receiveOffer(data.socketId, data.sdp);
            that.emit("get_offer", data);
        });

        this.on('_answer', function(data) {
            that.receiveAnswer(data.userid, data.sdp);
            that.emit('get_answer', data);
        });

        this.on('send_file_error', function(error, socketId, sendId, file) {
            that.cleanSendFile(sendId, socketId);
        });

        this.on('receive_file_error', function(error, sendId) {
            that.cleanReceiveFile(sendId);
        });

        this.on('ready', function() {
            that.createPeerConnections();
            that.addStreams();
            that.addDataChannels();
            that.sendOffers();
        });

		return 0;
    };


    /*************************流处理部分*******************************/


    //创建本地流
    anychat.prototype.createStream = function() {
        var that = this;

        var mediaConfig = {video:{mandatory: { minAspectRatio: 1.333, maxAspectRatio: 1.334 },
								optional: [{ maxFrameRate: 15 }, { minWidth: 320 }, { minHeigth: 240 }, { maxWidth: 1280 }, { maxHeigth: 720 }]
							}, audio:true}; 

        if (getUserMedia) {
            getUserMedia.call(navigator, mediaConfig, function(stream) {
                    that.localMediaStream = stream;
					that.localStatus.localMediaStream = stream;
                    if(that.localVideoObject) {
						if (navigator.mozGetUserMedia) {
							that.localVideoObject.mozSrcObject = stream;
							that.localVideoObject.play();
						}
						that.localVideoObject.src = URL.createObjectURL(stream);
						that.localVideoObject.play();
					}
                },
                function(error) {
                    that.emit("stream_create_error", error);
                });
        } else {
            that.emit("stream_create_error", new Error('WebRTC is not yet supported in this browser.'));
        }
    };

    //将本地流添加到所有的PeerConnection实例中
    anychat.prototype.addStreams = function() {
        var i, m,
            stream,
            connection;
        for (connection in this.peerConnections) {
            this.peerConnections[connection].addStream(this.localMediaStream);
        }
    };

    //将流绑定到video标签上用于输出
    anychat.prototype.attachStream = function(stream, domId) {
        var element = document.getElementById(domId);
        if (navigator.mozGetUserMedia) {
            element.mozSrcObject = stream;
            element.play();
        } else {
            element.src = webkitURL.createObjectURL(stream);
        }
        element.src = webkitURL.createObjectURL(stream);
    };


    /***********************信令交换部分*******************************/


    //向所有PeerConnection发送Offer类型信令
    anychat.prototype.sendOffers = function() {
        var i, m,
            pc,
            that = this,
            pcCreateOfferCbGen = function(pc, socketId) {
                return function(session_desc) {
                    pc.setLocalDescription(session_desc);
                    that.socket.send(JSON.stringify({
                        "eventName": "__offer",
                        "data": {
                            "sdp": session_desc,
                            "socketId": socketId
                        }
                    }));
                };
            },
            pcCreateOfferErrorCb = function(error) {
                console.log(error);
            };
        for (i = 0, m = this.connections.length; i < m; i++) {
            pc = this.peerConnections[this.connections[i]];
            pc.createOffer(pcCreateOfferCbGen(pc, this.connections[i]), pcCreateOfferErrorCb);
        }
    };

    //接收到Offer类型信令后作为回应返回answer类型信令
    anychat.prototype.receiveOffer = function(socketId, sdp) {
        var pc = this.peerConnections[socketId];
        this.sendAnswer(socketId, sdp);
    };

    //发送answer类型信令
    anychat.prototype.sendAnswer = function(socketId, sdp) {
        var pc = this.peerConnections[socketId];
        var that = this;
        pc.setRemoteDescription(new nativeRTCSessionDescription(sdp));
        pc.createAnswer(function(session_desc) {
            pc.setLocalDescription(session_desc);
            that.socket.send(JSON.stringify({
                "eventName": "__answer",
                "data": {
                    "socketId": socketId,
                    "sdp": session_desc
                }
            }));
        }, function(error) {
            console.log(error);
        });
    };

    //接收到answer类型信令后将对方的session描述写入PeerConnection中
    anychat.prototype.receiveAnswer = function(userid, sdp) {
		var pc = this.peerConnections[userid];
		var sdc = new nativeRTCSessionDescription();
		sdc.sdp = sdp;
		sdc.type = 'answer';
		pc.setRemoteDescription(sdc);
    };


    /***********************点对点连接部分*****************************/


    //创建与其他用户连接的PeerConnections
    anychat.prototype.createPeerConnections = function() {
        var i, m;
        for (i = 0, m = this.connections.length; i < m; i++) {
            this.createPeerConnection(this.connections[i]);
        }
    };

    //创建单个PeerConnection
    anychat.prototype.createPeerConnection = function(dwUserId) {
        var that = this;
		
		var pcOptional={optional:[{DtlsSrtpKeyAgreement: false}]};
		
        var pc = new PeerConnection(iceServer, pcOptional);
        this.peerConnections[dwUserId] = pc;
        pc.onicecandidate = function(evt) {
            if (evt.candidate)
                that.socket.send(JSON.stringify({
                    "eventName": "__ice_candidate",
                    "data": {
                        "label": evt.candidate.sdpMLineIndex,
                        "candidate": evt.candidate.candidate,
                        "userid": dwUserId
                    }
                }));
            that.emit("pc_get_ice_candidate", evt.candidate, dwUserId, pc);
        };

        pc.onopen = function() {
            that.emit("pc_opened", dwUserId, pc);
        };

        pc.onaddstream = function(evt) {
			console.log("onaddstream1");
			that.localStatus.remoteMediaStream = evt.stream;
			if(that.remoteVideoObject) {
				if (navigator.mozGetUserMedia) {
					that.remoteVideoObject.mozSrcObject = evt.stream;
					that.remoteVideoObject.play();
				}
				that.remoteVideoObject.src = URL.createObjectURL(evt.stream);
				that.remoteVideoObject.play();
			}
            that.emit('pc_add_stream', evt.stream, dwUserId, pc);
        };

        pc.ondatachannel = function(evt) {
            that.addDataChannel(dwUserId, evt.channel);
            that.emit('pc_add_data_channel', evt.channel, dwUserId, pc);
        };
        return pc;
    };

    //关闭PeerConnection连接
    anychat.prototype.closePeerConnection = function(pc) {
        if (!pc) return;
        pc.close();
    };
	

	// 获取版本信息
	anychat.prototype.GetVersion = function(type) {
		return "2.0.0.0";
	};
	
	anychat.prototype.GetSDKOptionString = function(optname) {
		return "";
	};
	
	anychat.prototype.SetServerAuthPass = function(lpPassword) {
		return 0;
	};
	
	anychat.prototype.Login = function(lpUserName, lpPassword, dwParam){
		var that = this;
		if(that.localStatus.socketConnect) {
			that.socket.sendCommand("login", {"username":lpUserName, "password":lpPassword, "param":parseInt(dwParam), "sdkauthpass":ls.sdkauthpass});
		}
		that.localStatus.loginuser = lpUserName;
		that.localStatus.loginpass = lpPassword;
		that.localStatus.loginparam = dwParam;
		return 0;
	};
	
	anychat.prototype.EnterRoom = function(dwRoomid, lpRoomPass, dwParam) {
		var that = this;
		if(that.localStatus.socketConnect) {
			that.socket.sendCommand("enterroom", {"roomid":parseInt(dwRoomid), "password":lpRoomPass, "param":parseInt(dwParam)});
		}
		that.localStatus.roomid = dwRoomid;
		that.localStatus.roompass = lpRoomPass;
		that.localStatus.roomparam = dwParam;
		return 0;
	}
	
	anychat.prototype.EnterRoomEx = function(lpRoomName, lpRoomPass) {
		var that = this;
		if(that.localStatus.socketConnect) {
			that.socket.sendCommand("enterroomex", {"roomname":lpRoomName, "password":lpRoomPass});
		}
		that.localStatus.roomname = lpRoomName;
		that.localStatus.roompass = lpRoomPass;
		return 0;
	}
	
	
	anychat.prototype.QueryUserStateString = function(dwUserId, dwOptName) {
		var that = this;
		if(typeof that.userInfoData[dwUserId] != "undefined") {
			if(dwOptName == BRAC_USERSTATE_NICKNAME)
				return that.userInfoData[dwUserId].nickname;
			else if(dwOptName == BRAC_USERSTATE_INTERNETIP)
				return that.userInfoData[dwUserId].internetip;
			else if(dwOptName == BRAC_USERSTATE_LOCALIP)
				return that.userInfoData[dwUserId].internetip;
		}
		return "";
	};
	
	anychat.prototype.QueryUserStateInt= function(dwUserId, dwOptName) {
		var that = this;
		if(typeof that.userInfoData[dwUserId] != "undefined") {
			if(dwOptName == BRAC_USERSTATE_LEVEL)
				return that.userInfoData[dwUserId].level;
		}
		return 0;
	};
	
	anychat.prototype.GetRoomOnlineUsers = function() {
		return this.roomusers;
	};
	
	anychat.prototype.LeaveRoom = function(dwRoomid) {
		var that = this;
		if(that.localStatus.socketConnect) {
			that.socket.sendCommand("leaveroom", {"roomid":parseInt(dwRoomid)});
		}
		clearRoomStatus();
		return 0;
	};
	
	anychat.prototype.Logout = function() {
		var that = this;
		if(that.localStatus.socketConnect) {
			that.socket.sendCommand("logout", {"errorcode":0});
		}
		that.socket.close();
		that.socket = null;
		that.localStatus.socketConnect = false;
		clearAllStatus();
		return 0;
	};
	
	anychat.prototype._attach_stream = function(parentobj, id, stream) {
		var that = this;
		var videoobj = document.getElementById(id);
		if(videoobj == null) {
			// 创建视频显示插件
			videoobj = document.createElement("video");
			videoobj.id = id;
			videoobj.style.width = "100%";
			videoobj.style.height= "100%";
			videoobj.autoplay = true;
			parentobj.appendChild(videoobj);
		}
		if(stream){
			if (navigator.mozGetUserMedia) {
				videoobj.mozSrcObject = stream;
				videoobj.play();
			}
			videoobj.src = URL.createObjectURL(stream);
		}
		return videoobj;
	}
	
	anychat.prototype.SetVideoPos = function(dwUserId, parentobj, id) {
		var that = this;
		if(!that.localStatus.socketConnect)
			return -1;
		if(dwUserId == -1 || that.localStatus.selfuserid == dwUserId) {
			that.localVideoObject = that._attach_stream(parentobj, id, that.localStatus.localMediaStream);
		} else {
			that.remoteVideoObject = that._attach_stream(parentobj, id, that.localStatus.remoteMediaStream);
		}
		return 0;
	};
	
	anychat.prototype.UserCameraControl = function(dwUserId, bOpen) {
		var that = this;
		if(!that.localStatus.socketConnect)
			return -1;
		that.socket.sendCommand("usercameracontrol", {"userid":parseInt(dwUserId), "bopen":parseInt(bOpen)});
		// 操作本地视频
		if(dwUserId == -1 || that.localStatus.selfuserid == dwUserId) {
			if(!!bOpen && !that.localMediaStream) {
				that.createStream();
			}
		} else {
			if(!!bOpen && typeof that.peerConnections[dwUserId] == "undefined") {
				var pc = that.createPeerConnection(dwUserId);
				pc.addStream(that.localMediaStream);
				pc.createOffer(function(desc) {
					pc.setLocalDescription(desc);
					that.socket.send(JSON.stringify({
						"eventName": "__offer",
						"data": {"sdp": desc.sdp, "userid": parseInt(dwUserId)}
					}));
				});
			} else if(!bOpen && typeof that.peerConnections[dwUserId] != "undefined") {
				
			}
		}
		return 0;
	};

	anychat.prototype.UserSpeakControl = function(dwUserId, bOpen) {
		var that = this;
		if(!that.localStatus.socketConnect)
			return -1;
		that.socket.sendCommand("userspeakcontrol", {"userid":parseInt(dwUserId), "bopen":parseInt(bOpen)});
		return 0;
	};
	
	// SDK内核参数设置
	anychat.prototype.SetSDKOptionString = function(optname, value) {
		var that = this;
		if(!that.localStatus.socketConnect) {
			var data = {"command":"setsdkoptionstring", "param":{"optname":parseInt(optname), "value":value}};
			that.sdkoptionData[optname] = data;
		} else {
			that.socket.sendCommand("setsdkoptionstring", {"optname":parseInt(optname), "value":value});
		}
		return 0;
	};
	// SDK内核参数设置
	anychat.prototype.SetSDKOptionInt = function(optname, value) {
		var that = this;		
		if(!that.localStatus.socketConnect) {
			var data = {"command":"setsdkoptionint", "param":{"optname":parseInt(optname), "value":parseInt(value)}};
			that.sdkoptionData[optname] = data;
		} else {
			that.socket.sendCommand("setsdkoptionint", {"optname":parseInt(optname), "value":parseInt(value)});
		}
	};
	
	// 视频呼叫事件控制（请求、回复、挂断等）
	anychat.prototype.VideoCallControl = function(dwEventType, dwUserId, dwErrorCode, dwFlags, dwParam, szUserStr) {
		var that = this;
		if(!that.localStatus.socketConnect)
			return -1;
		that.socket.sendCommand("videocallcontrol", {"event":parseInt(dwEventType), "userid":parseInt(dwUserId), "errorcode":parseInt(dwErrorCode), "flags":parseInt(dwFlags), "param":parseInt(dwParam), "userstr":szUserStr});
		return 0;
	};
	// 透明通道传送缓冲区
	anychat.prototype.TransBuffer = function(dwUserId, lpBuf, dwLen) {
		var that = this;
		if(!that.localStatus.socketConnect)
			return -1;
		that.socket.sendCommand("transbuffer", {"userid":parseInt(dwUserId), "buf":lpBuf});
		return 0;
	};
	// 传送文本消息
	anychat.prototype.SendTextMessage = function(dwUserId, bSecret, lpMsgBuf, dwLen) {
		var that = this;
		if(!that.localStatus.socketConnect)
			return -1;
		that.socket.sendCommand("sendtextmessage", {"userid":parseInt(dwUserId), "secret":parseInt(bSecret), "msgbuf":lpMsgBuf});
		return 0;
	};
	// 获取用户好友ID列表（返回一个userid的数组）
	anychat.prototype.GetUserFriends = function(){
		var that = this;
		var friends = new Array();
		for(var userid in that.friendInfoData)
			friends.push(userid);
		return friends;
	};
	// 获取好友在线状态
	anychat.prototype.GetFriendStatus = function(dwFriendUserId) {
		var that = this;
		if(typeof that.friendInfoData[dwFriendUserId] != "undefined")
			return that.friendInfoData[dwFriendUserId].status;
		else
			return 0;
	};
	// 获取用户分组ID列表（返回一个GroupId的数组）
	anychat.prototype.GetUserGroups = function() {
		var that = this;
		var groups = new Array();
		for(var groupid in that.groupInfoData)
			groups.push(groupid);
		return groups;
	};
	// 获取分组下面的好友列表（返回一个userid的数组）
	anychat.prototype.GetGroupFriends = function(dwGroupId) {
		var that = this;
		var friends = new Array();
		for(var userid in that.friendInfoData) {
			if(that.friendInfoData[userid].groupid == dwGroupId) {
				friends.push(userid);
			}
		}
		return friends;
	};
	// 获取用户信息
	anychat.prototype.GetUserInfo = function(dwUserId, dwInfoId) {
		var that = this;
		if(typeof that.friendInfoData[dwUserId] != "undefined")
			return that.friendInfoData[dwUserId]["t" + dwInfoId];
		else
			return "";
	};
	// 获取用户分组名称
	anychat.prototype.GetGroupName = function(dwGroupId) {
		var that = this;
		if(typeof that.groupInfoData[dwGroupId] != "undefined")
			return that.groupInfoData[dwGroupId].groupname;
		else
			return "";		
	};
	// 用户信息控制
	anychat.prototype.UserInfoControl = function(dwUserId, dwCtrlCode, wParam, lParam, lpStrValue) {
		var that = this;
		if(!that.localStatus.socketConnect)
			return -1;
		that.socket.sendCommand("userinfocontrol", {"userid":parseInt(dwUserId), "ctrlcode":parseInt(dwCtrlCode), "wparam":parseInt(wParam), "lparam":parseInt(lParam), "strvalue":lpStrValue});
		return 0;
	};
	// 获取业务对象ID列表（返回一个ObjectId的数组）
	anychat.prototype.ObjectGetIdList = function(dwObjectType) {
		var that = this;
		var objectids = new Array();
		for(var i in that.objectInfoData) {
			if(that.objectInfoData[i].objecttype == dwObjectType) {
				objectids.push(that.objectInfoData[i].objectid);
			}
		}
		return objectids;
	};
	// 获取业务对象参数值（整形值）
	anychat.prototype.GetObjectIntValue = function(dwObjectType, dwObjectId, dwInfoName) {
		var that = this;
		var value = 0;
		for(var i in that.objectInfoData) {
			if(that.objectInfoData[i].objecttype == dwObjectType && that.objectInfoData[i].objectid == dwObjectId) {
				if(dwInfoName == ANYCHAT_OBJECT_INFO_FLAGS)
					value = that.objectInfoData[i].flags;
				else if(dwInfoName == ANYCHAT_OBJECT_INFO_PRIORITY)
					value = that.objectInfoData[i].priority;
				else if(dwInfoName == ANYCHAT_OBJECT_INFO_ATTRIBUTE)
					value = that.objectInfoData[i].attribute;
				else if(dwInfoName == ANYCHAT_OBJECT_INFO_INTTAG)
					value = that.objectInfoData[i].inttag;
				else if(dwInfoName == ANYCHAT_QUEUE_INFO_WAITTIMESECOND)
					value = that.queueWaitSeconds;
				else {
					for(var k in that.objectInfoData[i].status) {
						if(parseInt(k) == dwInfoName) {
							value = parseInt(that.objectInfoData[i].status[k]);
							break;
						}
					}
				}
				break;
			}
		}
		return value;
	};
	// 获取业务对象参数值（字符串）
	anychat.prototype.GetObjectStringValue = function(dwObjectType, dwObjectId, dwInfoName) {
		var that = this;
		var value = "";
		for(var i in that.objectInfoData) {
			if(that.objectInfoData[i].objecttype == dwObjectType && that.objectInfoData[i].objectid == dwObjectId) {
				if(dwInfoName == ANYCHAT_OBJECT_INFO_NAME)
					value = that.objectInfoData[i].objectname;
				else if(dwInfoName == ANYCHAT_OBJECT_INFO_DESCRIPTION)
					value = that.objectInfoData[i].objectdesc;
				else if(dwInfoName == ANYCHAT_OBJECT_INFO_STRINGTAG)
					value = that.objectInfoData[i].strtag;
				else {
					for(var k in that.objectInfoData[i].status) {
						if(parseInt(k) == dwInfoName) {
							value = that.objectInfoData[i].status[k];
							break;
						}
					}
				}
				break;
			}
		}
		return value;
	};
	// 设置业务对象参数值
	anychat.prototype.SetObjectStringValue = function(dwObjectType, dwObjectId, dwInfoName, value) {
		var that = this;
		if(!that.localStatus.socketConnect)
			return -1;
		that.socket.sendCommand("setobjectstrvalue", {"objecttype":parseInt(dwObjectType), "objectid":parseInt(dwObjectId), "infoname":parseInt(dwInfoName), "value":value});
		return 0;
	};
	// 设置业务对象参数值
	anychat.prototype.SetObjectIntValue = function(dwObjectType, dwObjectId, dwInfoName, value) {
		var that = this;
		if(!that.localStatus.socketConnect)
			return -1;
		that.socket.sendCommand("setobjectintvalue", {"objecttype":parseInt(dwObjectType), "objectid":parseInt(dwObjectId), "infoname":parseInt(dwInfoName), "value":parseInt(value)});
		return 0;
	};
	// 业务对象控制指令
	anychat.prototype.ObjectControl = function(dwObjectType, dwObjectId, dwCtrlCode, dwParam1, dwParam2, dwParam3, dwParam4, strParam) {
		var that = this;
		if(!that.localStatus.socketConnect)
			return -1;
		that.socket.sendCommand("objectcontrol", {"objecttype":parseInt(dwObjectType), "objectid":parseInt(dwObjectId), "ctrlcode":parseInt(dwCtrlCode), 
				"param1":parseInt(dwParam1), "param2":parseInt(dwParam2), "param3":parseInt(dwParam3), "param4":parseInt(dwParam4), "strparam":strParam});
		return 0;
	};
	
	
    return new anychat();
};