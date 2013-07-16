package com.jovianware.jv8;


public class V8Runner {
  private V8Value Undefined_;
  public V8Value Undefined() {
    return V8Undefined.instance;
  }
  
  static {
    try {
      System.loadLibrary("gnustl_shared");
    } catch (UnsatisfiedLinkError e) {
      e.printStackTrace();
    }
    
    System.loadLibrary("jv8");
  }
  
  public static native void setDebuggingRunner(V8Runner v8, int port, boolean waitForConnection);
  
  public V8Value val(String str) {
    return new V8String(str);
  }
  
  public V8Value val(double num) {
    return new V8Number(num);
  }
  
  public V8Value val(boolean bool) {
    return new V8Boolean(bool);
  }
  
  private native long create();
  private native void dispose();

  private native V8Value callFunction(Function function, V8Value[] args) throws V8Exception;
  private native V8Value runJS(String name, String src) throws V8Exception;
  public native void printStackTrace();
  
  public V8Value tryRunJS(String name, String src) {
    try {
      return runJS(name, src);
    } catch (V8Exception e) {
      return null;
    }
  }

  // TODO: Let jv8 do the casting job on the JNI side.
  public Object temp_callFunction(Function function, Object[] args) throws V8Exception {
    V8Value[] arguments = new V8Value[args.length];
    for (int i=0; i<args.length; i++) {
      arguments[i] = castToV8Value(args[i]);
    }
    return castToJavaObject(callFunction(function, arguments));
  }

  // TODO: Let jv8 do the casting job on the JNI side.
  public Object temp_runJS(String name, String src) throws V8Exception {
    return castToJavaObject(runJS(name, src));
  }
  
  @Override
  public void finalize() {
    dispose();
  }
  
  public native void map(String name, V8MappableMethod m);
  
  private long handle;
  public V8Runner() {
    handle = create();
  }

  /**
   * ============= TYPE CONVERSION ==========
   */

  public static V8Value castToV8Value( Object obj ) {
    if( obj instanceof String ){
      return new V8String( (String)obj );
    }
    if( obj instanceof Number ){
      return new V8Number( ( (Number) obj ).doubleValue() );
    }
    if( obj instanceof Boolean ){
      return new V8Boolean( ( (Boolean) obj ).booleanValue() );
    }
    if( obj instanceof V8Value ){
      return (V8Value)obj;
    }
    if( obj == null ){
      return null;
    }
    throw new RuntimeException("Invalid type for castToV8Value: "+ obj.getClass().toString());
  }

  public static Object castToJavaObject( V8Value value ) {
    if (value == null) {
      return null;
    }
    else if (value instanceof V8Boolean) {
      return value.toBoolean();
    }
    else if (value instanceof V8String) {
      return value.toString();
    }
    else if (value instanceof V8Number) {
      return value.toNumber();
    }
    else if (value instanceof V8Function) {
      return value;
    }
    else if (value instanceof V8Undefined) {
      return value;
    }
    throw new RuntimeException("Invalid type for castToJavaObject: "+ value.getClass().toString());
  }
}
