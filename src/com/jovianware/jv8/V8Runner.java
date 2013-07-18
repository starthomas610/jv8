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

  public native Object callFunction(Function function, Object[] args) throws V8Exception;
  public native Object runJS(String name, String src) throws V8Exception;
  public native void printStackTrace();
  
  public Object tryRunJS(String name, String src) {
    try {
      return runJS(name, src);
    } catch (V8Exception e) {
      return null;
    }
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
}
