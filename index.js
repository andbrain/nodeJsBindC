var server = require('./server');
var currentPath = process.cwd();
var env 		= process.env.NODE_ENV || 'development';
var config 		= require(currentPath + '/config/config.json')[env];

//process.env.SERVER_PORT => port by command line
var port = Number(process.env.SERVER_PORT) || config.server_port;


server.listen(port, function () {
	console.log('Binder C++ as a Service listening on %j', server.address());
});