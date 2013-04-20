package com.jovianware.android;

import java.util.Arrays;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;

import com.jovianware.jv8.V8Exception;
import com.jovianware.jv8.V8MappableMethod;
import com.jovianware.jv8.V8Runner;
import com.jovianware.jv8.V8Value;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

public class JV8Test extends Activity {
  private V8Runner v8;
  
  // It's 2013, and Java still doesn't have a join() method in any of its standard utilities.
  static String join(Collection<?> s, String delimiter) {
    StringBuilder builder = new StringBuilder();
    Iterator<?> iter = s.iterator();
    while (iter.hasNext()) {
      builder.append(iter.next());
      if (!iter.hasNext()) {
        break;
      }
      builder.append(delimiter);
    }
    return builder.toString();
  }
  
  class TestMappableMethod implements V8MappableMethod {
    @Override
    public V8Value methodToRun(V8Value[] args) {
      Log.e("methodToRun", "Hello from Java!");
      Log.e("methodToRun", "Arguments:");
      for (V8Value val : args) {
        if (val.isArray()) {
          // The ordinary `toString` method works fine on Arrays, but
          //  I wanted to illustrate that `toArray` works well on its own. 
          String str = "[";
          List<V8Value> vals = Arrays.asList(val.toArray());
          str += join(vals, ", ");
          str += "]";
          Log.e("methodToRun", str);
        } else {
          Log.e("methodToRun", val.toString());
        }
      }
      return v8.val("TESTING RETURN VAL");
    }
  }
  
  @Override
  public void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.main);
    v8 = new V8Runner();
    
    try {
      v8.runJS("42;\n42+1;\nsomethingUndefined.runMe()");
    } catch (V8Exception e) {
      Log.e("V8Exception", e.getMessage());
    }
    
    v8.map(new TestMappableMethod(), "sayHello");
    v8.tryRunJS("sayHello('Testing', 1, 2, 3, [42,43,44]);");
  }
}
