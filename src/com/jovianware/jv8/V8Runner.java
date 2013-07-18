package com.jovianware.jv8;


public class V8Runner {
  private Undefined Undefined_;
  public Undefined Undefined() {
    return Undefined.instance;
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

  private native long create();
  private native void dispose();

  private native Object native_callFunction(Function function, Object[] args) throws V8Exception;
  private native Object native_runJS(String name, String src) throws V8Exception;
  public native void printStackTrace();
  
  public Object tryRunJS(String name, String src) {
    try {
      return native_runJS(name, src);
    } catch (V8Exception e) {
      return null;
    }
  }

  // TODO: Let jv8 do the casting job on the JNI side.
  public Object callFunction(Function function, Object[] args) throws V8Exception {
    return native_callFunction(function, args);
  }

  // TODO: Let jv8 do the casting job on the JNI side.
  public Object runJS(String name, String src) throws V8Exception {
    return native_runJS(name, src);
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

  /*
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
  */
}
