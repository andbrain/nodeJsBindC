#include <node.h>
#include <nan.h>

namespace demo {
  using v8::FunctionCallbackInfo;
  using v8::Isolate;
  using v8::Local;
  using v8::Object;
  using v8::String;
  using v8::Value;

  void Method(const FunctionCallbackInfo<Value>& args) {
    // Isolate* isolate = args.GetIsolate();
    // args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
    
    v8::String::Utf8Value nameFromArgs(args[0]->ToString());
    std::string name = std::string(*nameFromArgs);
    std::string response = "hello " + name;

    args.GetReturnValue().Set(Nan::New(response).ToLocalChecked());
  }

  void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "hello", Method);
  }

  NODE_MODULE(addon, init)
}