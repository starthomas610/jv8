package com.jovianware.jv8;

public abstract class V8MappableMethod {
  public abstract Object methodToRun(Object[] args);

  private Object runMethod(Object[] args){
    return methodToRun(args);
  }
}
