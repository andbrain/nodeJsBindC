{
  "targets": [
    {
    	"target_name": "addon",
    	"sources": [ 
    		"binder.cc",
        "c/engine.cpp",
        "c/tads/document.cpp",
        "c/tads/qprocessor.cpp",
        "c/tads/util.cpp",
    		"c/tads/ireader.cpp",
    	],
  		"include_dirs" : [
        "c"
      ]
    }
  ]
}