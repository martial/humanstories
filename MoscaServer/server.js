var mosca = require('mosca');

var ascoltatore = {
    //using ascoltatore
    type: 'mongo',        
    url: 'mongodb://localhost:27017/mqtt',
    pubsubCollection: 'ascoltatori',
    mongo: {},
};

var moscaSettings = {
    port: 1883,
    backend: ascoltatore,
    persistence: {
        factory: mosca.persistence.Mongo,
        url: 'mongodb://localhost:27017/mqtt'
    }
};

var server = new mosca.Server(moscaSettings);
server.on('ready', setup);

server.on('clientConnected', function (client) {
    console.log('client connected', client.id);
     var message = {
  		topic: 'connect',
  		payload: client.id, // or a Buffer
  		qos: 0, // 0, 1, or 2
  		retain: false // or true
	};
	 server.publish(message, function() {});
});

server.on('clientDisconnected', function(client) {

		var message = {
                topic: "disconnect",
                payload: client.id, // or a Buffer
                qos: 0, // 0, 1, or 2
                retain: true // or true
                };

                server.publish(message, function() {
                      console.log("disconnect " + client.id);
                 });       

});

// fired when a message is received
server.on('published', function (packet, client) {
          
                                    console.log("message " + packet.payload + " – topic "+ packet.topic);

   
	if(client && client.id == "openframeworks") {

                          console.log("Command " + packet.payload + " – topic "+ packet.topic);


			//if(packet.topic == "command") {
/*
			 var message = {
                topic: packet.topic,
                payload: packet.payload, // or a Buffer
                qos: 0, // 0, 1, or 2
                retain: true // or true
                };

                server.publish(message, function() {
                      console.log("Command " + packet.payload + " – topic "+ packet.topic);
                 });

            //}
            */
	}
           

});

// fired when the mqtt server is ready
function setup() {
    console.log('Mosca server is up and running');
}