#include <iostream>
#include <cstdlib>
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

const auto SWIFT_REGISTER_FOR_CHANGES_FUNC_SYMBOL = "register_for_changes";

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

int main() {
  const auto swiftLib = DLOpenOrDie(SWIFT_SHARED_LIBRARY_PATH);
  const auto _RegisterForChangesFunc = (RegisterForChangesFunc)DLSymOrDie(swiftLib, SWIFT_REGISTER_FOR_CHANGES_FUNC_SYMBOL);

  std::cout << "About to invoke swift code..." << std::endl;
  _RegisterForChangesFunc(testCB);
  std::cout << "Finished invoking swift code." << std::endl;
}
