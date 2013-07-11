#include <jni.h>

#include "JNIUtil.h"

namespace jv8 {

// Fields
jfieldID JNIUtil::f_V8Runner_handle = NULL;
jfieldID JNIUtil::f_V8Function_handle = NULL;
jfieldID JNIUtil::f_V8Function_runnerHandle = NULL;
jfieldID JNIUtil::f_V8Boolean_val = NULL;
jfieldID JNIUtil::f_V8Number_val = NULL;
jfieldID JNIUtil::f_V8String_val = NULL;

// Static fields
jfieldID JNIUtil::sf_V8Undefined_instance = NULL;
jfieldID JNIUtil::sf_V8Value_TYPE_BOOLEAN = NULL;
jfieldID JNIUtil::sf_V8Value_TYPE_NUMBER = NULL;
jfieldID JNIUtil::sf_V8Value_TYPE_STRING = NULL;
jfieldID JNIUtil::sf_V8Value_TYPE_FUNCTION = NULL;
jfieldID JNIUtil::sf_V8Value_TYPE_UNDEFINED;

// Methods
jmethodID JNIUtil::m_V8String_init_str = NULL;
jmethodID JNIUtil::m_V8Number_init_num = NULL;
jmethodID JNIUtil::m_V8Boolean_init_bool = NULL;
jmethodID JNIUtil::m_V8Function_init = NULL;
jmethodID JNIUtil::m_V8MappableMethod_methodToRun = NULL;
jmethodID JNIUtil::m_V8Value_getTypeID = NULL;

// Classes
jclass JNIUtil::V8Runner_class = NULL;
jclass JNIUtil::V8Value_class = NULL;
jclass JNIUtil::V8String_class = NULL;
jclass JNIUtil::V8Number_class = NULL;
jclass JNIUtil::V8Boolean_class = NULL;
jclass JNIUtil::V8Function_class = NULL;
jclass JNIUtil::V8Undefined_class = NULL;
jclass JNIUtil::V8Exception_class = NULL;
jclass JNIUtil::V8MappableMethod_class = NULL;

// Types
int JNIUtil::V8VALUE_TYPE_BOOLEAN = 0;
int JNIUtil::V8VALUE_TYPE_NUMBER = 0;
int JNIUtil::V8VALUE_TYPE_STRING = 0;
int JNIUtil::V8VALUE_TYPE_FUNCTION = 0;
int JNIUtil::V8VALUE_TYPE_UNDEFINED = 0;

// Misc
bool JNIUtil::needsToCacheClassData = true;
JavaVM* JNIUtil::javaVM = NULL;

}