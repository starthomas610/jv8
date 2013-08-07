package com.jovianware.jv8;

public class V8Exception extends Exception {
  private static final long serialVersionUID = 1L;

  private static final int UNKNOWN_LINE_NUMBER = -1;
  private static final String UNKNOWN_FILENAME = null;

  private String filename;
  private int lineNumber;

  private V8Exception(String msg) {
    this(msg, UNKNOWN_FILENAME, UNKNOWN_LINE_NUMBER);
  }

  private V8Exception(String msg, String filename, int lineNumber) {
    super(msg);
    this.filename = filename;
    this.lineNumber = lineNumber;
  }

  public String getFilename() {
    return filename;
  }

  public int getLineNumber() {
    return lineNumber;
  }

  @Override
  public String toString(){
    return getMessage() +" ("+ filename +":"+ lineNumber +")";
  }
}
