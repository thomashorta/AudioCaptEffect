// FileChooser based on code by H3R3TIC from dream in code forum
// link http://www.dreamincode.net/forums/topic/190013-creating-simple-file-chooser/

package com.test.audiocapteffect;

import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Stack;

import android.app.ListActivity;
import android.content.Intent;
import android.media.AudioManager;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.ListView;

public class FileChooser extends ListActivity {

	private File currentDir;
	private FileArrayAdapter adapter;
	Stack<File> dirStack = new Stack<File>();

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setVolumeControlStream(AudioManager.STREAM_MUSIC);
		
		currentDir = new File(Environment.getExternalStorageDirectory()
				.getPath());
		fill(currentDir);
	}
	
	@Override
	public void onBackPressed() {
		if (dirStack.size() == 0) {
			Intent i = new Intent();
			i.putExtra("filename", "INVALID");
			setResult(RESULT_CANCELED, i);
			finish();
			return;
		}
		currentDir = dirStack.pop();
		fill(currentDir);
	}

	private void fill(File f) {
		File[] dirs = f.listFiles();
		String dirName;
		
		if (f.getName().equals("0"))
			dirName="sdcard";
		else 
			dirName = f.getName();
		this.setTitle("Current Dir: " + dirName);
		List<Option> dir = new ArrayList<Option>();
		List<Option> fls = new ArrayList<Option>();
		try {
			for (File ff : dirs) {
				if (ff.isDirectory())
					dir.add(new Option(ff.getName(), "Folder", ff
							.getAbsolutePath()));
				else {
					fls.add(new Option(ff.getName(), "File Size: "
							+ convertSize(ff.length()), ff.getAbsolutePath()));
				}
			}
		} catch (Exception e) {

		}
		Collections.sort(dir);
		Collections.sort(fls);
		dir.addAll(fls);
		if (!dirName.equalsIgnoreCase("sdcard"))
			dir.add(0, new Option("..", "Parent Directory", f.getParent()));
		adapter = new FileArrayAdapter(FileChooser.this, R.layout.file_view,
				dir);
		this.setListAdapter(adapter);
	}
	
	static String convertSize(long bytelen) {
		String size;

		if (bytelen > 1073741824) {
			size = String.format("%.2f Gb", (float) bytelen / 1073741824);
		} else if (bytelen > 1048576) {
			size = String.format("%.2f Mb", (float) bytelen / 1048576);
		} else if (bytelen > 1024) {
			size = String.format("%.2f Kb", (float) bytelen / 1024);
		} else {
			size = String.format("%d Kb", bytelen);
		}

		return size;
	}

	@Override
	protected void onListItemClick(ListView l, View v, int position, long id) {
		super.onListItemClick(l, v, position, id);
		Option o = adapter.getItem(position);
		if (o.getData().equalsIgnoreCase("folder")) {
			dirStack.push(currentDir);
			currentDir = new File(o.getPath());
			fill(currentDir);
		} else if (o.getData().equalsIgnoreCase("parent directory")) {
			currentDir = dirStack.pop();
			fill(currentDir);
		} else {
			onFileClick(o);
		}
	}

	private void onFileClick(Option o) {
		// Create intent to return data (Strings filename and filepath)
		Intent i = new Intent();
		i.putExtra("filename", o.getName());
		i.putExtra("filepath", o.getPath());
		
		setResult(RESULT_OK, i);
		
		finish();
	}




}
