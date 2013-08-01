// FileChooser based on code by H3R3TIC from dream in code forum
// link http://www.dreamincode.net/forums/topic/190013-creating-simple-file-chooser/

package com.test.audiocapteffect;

import android.annotation.SuppressLint;

public class Option implements Comparable<Option>{
    private String name;
    private String data;
    private String path;
    
    public Option(String n,String d,String p)
    {
        name = n;
        data = d;
        path = p;
    }
    public String getName()
    {
        return name;
    }
    public String getData()
    {
        return data;
    }
    public String getPath()
    {
        return path;
    }
    @SuppressLint("DefaultLocale")
	@Override
    public int compareTo(Option o) {
        if(this.name != null)
            return this.name.toLowerCase().compareTo(o.getName().toLowerCase()); 
        else 
            throw new IllegalArgumentException();
    }
}

