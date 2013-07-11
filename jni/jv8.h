#ifndef JV8_H_
#define JV8_H_
#include <jni.h>

#ifdef __ANDROID__
#include <android/log.h>
#endif

#include <v8.h>
using namespace v8;

#include "JNIUtil.h"

namespace jv8 {

class V8Runner;

struct MappableMethodData {
  jobject methodObject;
  V8Runner* runner;
  JavaVM* jvm;
};

#define REQUIRE_CLASS(className, globalRef)\
  if (!globalRef) {\
    jclass klass = NULL;\
    klass = (env)->FindClass(className);\
    globalRef = reinterpret_cast<jclass>(env->NewGlobalRef(klass));\
    env->DeleteLocalRef(klass);\
  }\

#define REQUIRE_METHOD(klass, method, signature, methodID)\
  if (!methodID) {\
    (methodID) = env->GetMethodID(klass, (method), (signature));\
  }\

#define REQUIRE_FIELD(klass, field, signature, fieldID)\
  if (!fieldID) {\
    (fieldID) = env->GetFieldID(klass, (field), (signature));\
  }\

#define REQUIRE_STATIC_FIELD(klass, field, signature, fieldID)\
  if (!fieldID) {\
    (fieldID) = env->GetStaticFieldID(klass, (field), (signature));\
  }\

#define THROW_V8EXCEPTION(env, tryCatch)\
  if (!tryCatch.HasCaught()) {\
    env->ThrowNew(JNIUtil::V8Exception_class, "Unexpected Error running JS");\
  }\
  else {\
    Persistent<Value> ex = Persistent<Value>::New(isolate, tryCatch.StackTrace());\
    env->ThrowNew(JNIUtil::V8Exception_class, *String::Utf8Value(ex->ToString()));\
    ex.Dispose(isolate);\
  }\

static inline void
cacheClassData(JNIEnv* env) {

  __android_log_print(ANDROID_LOG_INFO, "jv8", "Caching JNI methods");

  REQUIRE_CLASS("com/jovianware/jv8/V8Value", JNIUtil::V8Value_class)
  REQUIRE_METHOD(JNIUtil::V8Value_class, "getTypeID", "()I", JNIUtil::m_V8Value_getTypeID)
  REQUIRE_STATIC_FIELD(JNIUtil::V8Value_class, "TYPE_BOOLEAN", "I", JNIUtil::sf_V8Value_TYPE_BOOLEAN)
  REQUIRE_STATIC_FIELD(JNIUtil::V8Value_class, "TYPE_NUMBER", "I", JNIUtil::sf_V8Value_TYPE_NUMBER)
  REQUIRE_STATIC_FIELD(JNIUtil::V8Value_class, "TYPE_STRING", "I", JNIUtil::sf_V8Value_TYPE_STRING)
  REQUIRE_STATIC_FIELD(JNIUtil::V8Value_class, "TYPE_FUNCTION", "I", JNIUtil::sf_V8Value_TYPE_FUNCTION)
  REQUIRE_STATIC_FIELD(JNIUtil::V8Value_class, "TYPE_UNDEFINED", "I", JNIUtil::sf_V8Value_TYPE_UNDEFINED)

  JNIUtil::V8VALUE_TYPE_BOOLEAN = env->GetStaticIntField(JNIUtil::V8Value_class, JNIUtil::sf_V8Value_TYPE_BOOLEAN);
  JNIUtil::V8VALUE_TYPE_NUMBER = env->GetStaticIntField(JNIUtil::V8Value_class, JNIUtil::sf_V8Value_TYPE_NUMBER);
  JNIUtil::V8VALUE_TYPE_STRING = env->GetStaticIntField(JNIUtil::V8Value_class, JNIUtil::sf_V8Value_TYPE_STRING);
  JNIUtil::V8VALUE_TYPE_FUNCTION = env->GetStaticIntField(JNIUtil::V8Value_class, JNIUtil::sf_V8Value_TYPE_FUNCTION);
  JNIUtil::V8VALUE_TYPE_UNDEFINED = env->GetStaticIntField(JNIUtil::V8Value_class, JNIUtil::sf_V8Value_TYPE_UNDEFINED);

  // V8String
  REQUIRE_CLASS("com/jovianware/jv8/V8String", JNIUtil::V8String_class)
  REQUIRE_METHOD(JNIUtil::V8String_class, "<init>", "(Ljava/lang/String;)V", JNIUtil::m_V8String_init_str)
  REQUIRE_FIELD(JNIUtil::V8String_class, "val", "Ljava/lang/String;", JNIUtil::f_V8String_val)

  // V8Undefined
  REQUIRE_CLASS("com/jovianware/jv8/V8Undefined", JNIUtil::V8Undefined_class)
  REQUIRE_STATIC_FIELD(JNIUtil::V8Undefined_class, "instance", "Lcom/jovianware/jv8/V8Undefined;", JNIUtil::sf_V8Undefined_instance)

  // V8Number
  REQUIRE_CLASS("com/jovianware/jv8/V8Number", JNIUtil::V8Number_class)
  REQUIRE_METHOD(JNIUtil::V8Number_class, "<init>", "(D)V", JNIUtil::m_V8Number_init_num)
  REQUIRE_FIELD(JNIUtil::V8Number_class, "val", "D", JNIUtil::f_V8Number_val)

  // V8Boolean
  REQUIRE_CLASS("com/jovianware/jv8/V8Boolean", JNIUtil::V8Boolean_class)
  REQUIRE_METHOD(JNIUtil::V8Boolean_class, "<init>", "(Z)V", JNIUtil::m_V8Boolean_init_bool)
  REQUIRE_FIELD(JNIUtil::V8Boolean_class, "val", "Z", JNIUtil::f_V8Boolean_val)

  // V8Function
  REQUIRE_CLASS("com/jovianware/jv8/V8Function", JNIUtil::V8Function_class)
  REQUIRE_METHOD(JNIUtil::V8Function_class, "<init>", "(JJ)V", JNIUtil::m_V8Function_init)
  REQUIRE_FIELD(JNIUtil::V8Function_class, "handle", "J", JNIUtil::f_V8Function_handle)
  REQUIRE_FIELD(JNIUtil::V8Function_class, "runnerHandle", "J", JNIUtil::f_V8Function_runnerHandle)

  REQUIRE_CLASS("com/jovianware/jv8/V8MappableMethod", JNIUtil::V8MappableMethod_class)
  REQUIRE_METHOD(JNIUtil::V8MappableMethod_class, "methodToRun", "([Lcom/jovianware/jv8/V8Value;)Lcom/jovianware/jv8/V8Value;", JNIUtil::m_V8MappableMethod_methodToRun)

  JNIUtil::needsToCacheClassData = false;
}

jobject
newV8Value (
  JNIEnv* env,
  V8Runner* runner,
  Handle<Value> value
);

Handle<Value>
registerCallback (const Arguments& args);

} // namespace jv8

#endif // JV8_H_