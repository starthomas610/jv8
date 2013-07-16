package com.jovianware.jv8;

public abstract class V8MappableMethod {
  public abstract Object methodToRun(Object[] args);

  private V8Value runMethod(V8Value[] args){
    Object[] arguments = new Object[args.length];
    for (int i=0; i<args.length; i++){
      arguments[i] = V8Runner.castToJavaObject(args[i]);
    }
    return V8Runner.castToV8Value(methodToRun(arguments));
  }
}
