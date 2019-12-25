#include <iostream>
#include <cstdlib>
#include <napi.h>
#include <dlfcn.h>

using std::cerr;
using std::endl;
using std::string;

#ifdef __APPLE__
#define IS_SUPPORTED_PLATFORM true
const string SHARED_LIBRARY_EXT = "dylib";
#endif

#ifdef __linux__
#define IS_SUPPORTED_PLATFORM true
const string SHARED_LIBRARY_EXT = "so";
#endif

#if !defined IS_SUPPORTED_PLATFORM
  cerr << "Unsupported Platform";
  exit(EXIT_FAILURE);
#endif

const string SWIFT_SHARED_LIBRARY_PATH = "./src/CallbackExample/.build/debug/libCallbackExample."+SHARED_LIBRARY_EXT;

const auto SWIFT_FIBONACCI_FUNC_SYMBOL = "swift_fibonacci";
const auto SWIFT_REGISTER_FOR_CHANGES_FUNC_SYMBOL = "register_for_changes";

typedef int (*FibonacciFunc)(int);
typedef bool (*RegisterForChangesFunc)(std::function<void(char *)>);

void *DLSymOrDie(void *lib, const string& func_name) {
  const auto func = dlsym(lib, func_name.c_str());
  const auto dlsym_error = dlerror();
  if (dlsym_error) {
    cerr << "Could not load symbol create: " << dlsym_error << endl;
    dlclose(lib);
    exit(EXIT_FAILURE);
  }
  return func;
}


void *DLOpenOrDie(const string& path) {
  const auto lib = dlopen(path.c_str(), RTLD_LAZY);
  if (!lib) {
    cerr << "Could not load library: " << dlerror() << endl;
    exit(EXIT_FAILURE);
  }
  return lib;
}

void testCB(const char *name) {
  printf("Name: %s\n", name);
}

Napi::Value Fibonacci(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1) {
    Napi::TypeError::New(env, "Wrong number of arguments")
      .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Argument should be a number")
      .ThrowAsJavaScriptException();
    return env.Null();
  }

  const auto swiftLib = DLOpenOrDie(SWIFT_SHARED_LIBRARY_PATH);
  const auto _Fibonacci = (FibonacciFunc)DLSymOrDie(swiftLib, SWIFT_FIBONACCI_FUNC_SYMBOL);

  double arg0 = info[0].As<Napi::Number>().DoubleValue();
  return Napi::Number::New(env, _Fibonacci(arg0));
}

Napi::Boolean RegisterForChanges(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1) {
    Napi::TypeError::New(env, "Wrong number of arguments")
      .ThrowAsJavaScriptException();
    return Napi::Boolean();
  }

  const auto swiftLib = DLOpenOrDie(SWIFT_SHARED_LIBRARY_PATH);
  const auto _RegisterForChangesFunc = (RegisterForChangesFunc)DLSymOrDie(swiftLib, SWIFT_REGISTER_FOR_CHANGES_FUNC_SYMBOL);

  std::cout << "About to invoke swift code..." << std::endl;
  const auto result = Napi::Boolean::New(env, _RegisterForChangesFunc(testCB));
  std::cout << "Finished invoking swift code." << std::endl;

  return result;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "fibonacci"),
              Napi::Function::New(env, Fibonacci));
  exports.Set(Napi::String::New(env, "registerForChanges"),
              Napi::Function::New(env, RegisterForChanges));
  return exports;
}

NODE_API_MODULE(swift, Init)
