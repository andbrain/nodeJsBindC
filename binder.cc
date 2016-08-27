#include <node.h>
// #include <nan.h>
#include <iostream>
// #include "test.h"
#include "c/engine.h"

class MyEngine // or whatever name you like
{
public:
   MyEngine(){ m_engine.init(); }//or create it here? whatever solution suits your way of creating the data

   // ~MyEngine() { delete m_engine; } // THAT's the important part!

    Engine getInstance() const { return m_engine; } // or whatever accessor you need, if you need one

private:
    Engine m_engine;
};

// static MyEngine engine;

std::vector<int> ProcessQuery(std::string query){
  std::vector<int> resp;
  

  Engine e;
  e.init();
  resp = e.process(query);
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

  void Create(const FunctionCallbackInfo<Value>& args) {
    args.GetReturnValue();
  }

  void Close(const FunctionCallbackInfo<Value>& args) {
    args.GetReturnValue();
  }

  void Process(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    
    // Make sure there is an argument
    if(args.Length() != 1){
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Need an argument!")));
      return;
    }

    // convert first arg to std::string and call c++ function
    v8::String::Utf8Value nameFromArgs(args[0]->ToString());
    std::string query = std::string(*nameFromArgs);

    std::vector<int> vec = ProcessQuery(query);

    // Pack std::vector into a JS array
    Local<Array> result = Array::New(isolate);
    for (unsigned int i = 0; i < vec.size(); ++i)
    {
      result->Set(i, Number::New(isolate, vec[i]));
    }

    args.GetReturnValue().Set(result);
  }

  void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "init", Create);
    NODE_SET_METHOD(exports, "process", Process);
    NODE_SET_METHOD(exports, "close", Close);
  }

  NODE_MODULE(addon, init)
}