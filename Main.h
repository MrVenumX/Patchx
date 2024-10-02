#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <cstring>
#include <thread>
#include <unistd.h>
#include <dlfcn.h>
#include <jni.h>

const char *libName = "libtarget.so";
uintptr_t libBase;