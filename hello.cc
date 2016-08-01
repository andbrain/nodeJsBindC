#include <node.h>
// #include <nan.h>
#include <iostream>
// #include "test.h"
#include "c/engine.h"

std::vector<int> ReturnArrayFromName(std::string query){
  std::vector<int> resp;

  Engine *e = new Engine();
  e->init();
  resp = e->process("Is CF mucus abnormal?");
  // resp.push_back(1);
  // resp.push_back(3);
  delete e;
  return resp;
}

namespace demo {
  using v8::FunctionCallbackInfo;
  using v8::Isolate;
  using v8::Local;
  using v8::Object;
  using v8::String;
  using v8::Value;

  using v8::Number;
  using v8::Array;
  using v8::Exception;

  void Method(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    
    // Make sure there is an argument
    if(args.Length() != 1){
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Need an argument!")));
      return;
    }

    // convert first arg to std::string and call c++ function
    v8::String::Utf8Value nameFromArgs(args[0]->ToString());
    std::string query = std::string(*nameFromArgs);
    std::vector<int> vec = ReturnArrayFromName(query);


    // Pack std::vector into a JS array
    Local<Array> result = Array::New(isolate);
    for (unsigned int i = 0; i < vec.size(); ++i)
    {
      result->Set(i, Number::New(isolate, vec[i]));
    }

    args.GetReturnValue().Set(result);
  }

  void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "hello", Method);
  }

  NODE_MODULE(addon, init)
}