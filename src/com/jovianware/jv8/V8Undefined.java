package com.jovianware.jv8;

public class V8Undefined extends V8Value {

  public static V8Undefined instance = new V8Undefined();

  private V8Undefined(){

  }

  @Override
  public boolean isUndefined() {
    return true;
  }

  @Override
  public boolean toBoolean() {
    return false;
  }

  @Override
  public double toNumber() {
    return Double.NaN;
  }

  @Override
  public int getTypeID() {
    return V8Value.TYPE_UNDEFINED;
  }

}
