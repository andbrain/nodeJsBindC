{
  "targets": [
    {
      	"target_name": "addon",
      	"sources": [ "hello.cc", "test.cpp" ],
  		"include_dirs" : ["<!(node -e \"require('nan')\")"]
    }
  ]
}