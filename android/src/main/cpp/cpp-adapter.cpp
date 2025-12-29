#include <jni.h>
#include "NitroSelectableTextOnLoad.hpp"

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*) {
  return margelo::nitro::nitroselectabletext::initialize(vm);
}
