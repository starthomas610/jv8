#ifndef JNIUTIL_H_
#define JNIUTIL_H_

#include <sys/types.h>
#include <jni.h>

namespace jv8 {

/**
 * Utility class that stores all the cached field/method/class
 * to share pointers accross files.
 */
class JNIUtil {
public:
  static jfieldID f_V8Runner_handle,
                  f_V8Function_handle,
                  f_V8Function_runnerHandle,
                  f_V8Boolean_val,
                  f_V8Number_val,
                  f_V8String_val,
                  sf_V8Undefined_instance,
                  sf_V8Value_TYPE_BOOLEAN,
                  sf_V8Value_TYPE_NUMBER,
                  sf_V8Value_TYPE_STRING,
                  sf_V8Value_TYPE_FUNCTION,
                  sf_V8Value_TYPE_UNDEFINED;

  static jmethodID m_V8String_init_str,
                   m_V8Number_init_num,
                   m_V8Boolean_init_bool,
                   m_V8Function_init,
                   m_V8MappableMethod_runMethod,
                   m_V8Value_getTypeID;

  static jclass V8Runner_class,
                V8Value_class,
                V8String_class,
                V8Number_class,
                V8Boolean_class,
                V8Function_class,
                Function_class,
                V8Undefined_class,
                V8Exception_class,
                V8MappableMethod_class;

  static bool needsToCacheClassData;
  static JavaVM* javaVM;

  static int V8VALUE_TYPE_BOOLEAN,
             V8VALUE_TYPE_NUMBER,
             V8VALUE_TYPE_STRING,
             V8VALUE_TYPE_FUNCTION,
             V8VALUE_TYPE_UNDEFINED;

};

}

#endif