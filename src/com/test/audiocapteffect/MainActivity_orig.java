package com.test.audiocapteffect;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioRecord;
import android.media.AudioTrack;
import android.media.MediaRecorder;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.text.Html;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

// ORIGINAL ACTIVITY
public class MainActivity_orig extends Activity {
	
	final public String TAG = "AudioCaptEffect";

	public int SAMPLERATE;
	public double FFT_percent;
	
	public boolean isRecording = false, isPlaying = false;
	public TextView instrText, status, info;
	public Spinner SR;
	public ToggleButton recButton;
	public Button loadButton, playButton, fftButton;
	public Thread recThread;
	public AudioManager am;
	public int time = 0;
	public String filename, filepath, fileinfo;
	
	static {
        System.loadLibrary("nativefunc");
    }
	
	public native short[] addeffects(short[] input, int samplerate, int inputsize);
	
	public native double[] absfft60ms(double[] input, int samplerate);
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		setVolumeControlStream(AudioManager.STREAM_MUSIC);
		
		// Get the Views
		instrText = (TextView) findViewById(R.id.instrText);
		recButton = (ToggleButton) findViewById(R.id.recButton);
		loadButton = (Button) findViewById(R.id.loadButton);
		playButton = (Button) findViewById(R.id.playButton);
		fftButton = (Button) findViewById(R.id.fftButton);
		status = (TextView) findViewById(R.id.status);
		info = (TextView) findViewById(R.id.info);
		SR = (Spinner) findViewById(R.id.SR_spinner);

		// Set the texts for instrText and recButton
		instrText.setText("This app records audio and plays it back with reverb."
						+ "\nPress the Record button to start Recording."
						+ "\nPress again to stop recording and play the audio.");
		recButton.setChecked(false);
		
		// Initialize spinner
		spinner_init();
		
		am = (AudioManager)getSystemService(Context.AUDIO_SERVICE);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		//getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	@Override
	protected void onStop() {
		super.onStop();
		File file = new File(getCacheDir() + "/reverseme.pcm");

		// Delete any previous recording.
		if (file.exists())	file.delete();
	}
	
	public void onClick_load(View view) {
		Intent i = new Intent("com.test.audiocapteffect.FileChooser");
		startActivityForResult(i, 1);
	}

	public void onClick_record(View v) {
		
		// Get the sample rate from the EditText view
		SAMPLERATE = Integer.parseInt(SR.getSelectedItem().toString());
		
		if (isPlaying) {
			recButton.toggle();
			Toast toast = Toast.makeText(getApplicationContext(), "The audio is currently playing.\nPlease wait...", Toast.LENGTH_SHORT);
			toast.show();			
		}
		
		else if (recButton.isChecked()) {
			isRecording = true;
			loadButton.setEnabled(false);
			playButton.setEnabled(false);
			fftButton.setEnabled(false);
			
			recThread =  new Thread(new Runnable() {
				public void run() {
					record();
				}
			});
			recThread.start();
		}

		else {
			isRecording = false;
			loadButton.setEnabled(true);
			playButton.setEnabled(true);
			fftButton.setEnabled(true);

			try {
				recThread.join();
			} catch (InterruptedException e) {}
		}
	}
	
	public void onClick_play(View view) {
		loadButton.setEnabled(false);
		playButton.setEnabled(false);
		recButton.setEnabled(false);
		fftButton.setEnabled(false);
		
		Thread playThread = new Thread(new Runnable() {
			public void run() {
				play();
				runOnUiThread(new Runnable() {
					public void run(){
						loadButton.setEnabled(true);
						playButton.setEnabled(true);
						recButton.setEnabled(true);
						fftButton.setEnabled(true);
					}
				});
			}
		});
		playThread.start();
		
		/*try{
			playThread.join();
		} catch(Exception e) {}*/
	}
	
	public void onClick_fft(View view){
		loadButton.setEnabled(false);
		playButton.setEnabled(false);
		recButton.setEnabled(false);
		fftButton.setEnabled(false);
		
		Thread playThread = new Thread(new Runnable() {
			public void run() {
				takeFFT();
				runOnUiThread(new Runnable() {
					public void run(){
						loadButton.setEnabled(true);
						playButton.setEnabled(true);
						recButton.setEnabled(true);
						fftButton.setEnabled(true);
					}
				});
			}
		});
		playThread.start();
	}
	
	public void load() {
		if (!filepath.equals("")) {
			
			runOnUiThread(new Runnable() {
				public void run() {
					status.setText("Loading " + filename + "...\nPlease wait.");
				}
			});	
			
			// read the wavefile and converts it to raw data byte[]
			wavIO wavfile = new wavIO(filepath);
			byte[] wavdata = wavfile.read();
			
			// set the new Sample Rate to be the same as the file
			SAMPLERATE = wavfile.getSampleRate();
			
			// set the time length of the loaded file
			time = wavfile.getTimeInSec();
			
			// create file streams to save the raw data to temp.pcm
			File file = new File(getCacheDir() + "/temp.pcm");
			
			// Delete any previous temp file.
			if (file.exists())
				file.delete();

			// Create the new file.
			try {
				file.createNewFile();
			} catch (IOException e) {
				throw new IllegalStateException("Failed to create "
						+ file.toString());
			}

			try {
				// Create a DataOuputStream to write the audio data into the saved
				// file.
				OutputStream os = new FileOutputStream(file);
				BufferedOutputStream bos = new BufferedOutputStream(os);
				DataOutputStream dos = new DataOutputStream(bos);
				
				// Big endian conversion of the wav data to the pcm file
				for (int i = 0; i < wavdata.length; i = i+2) {
					int low = wavdata[i] & 0xff;
					int high = wavdata[i+1] & 0xff;
					short temp = (short)( high << 8 | low );
					dos.writeShort(temp);
				}
				
				dos.close();
				
			} catch(Throwable t) {
				Log.e("LoadWav", "Failed to convert wav to pcm");
			}
			
			runOnUiThread(new Runnable() {
				public void run() {
					status.setText(filename + " was successfully loaded.");
					
				}
			});	
			
			updateinfo(wavfile.getSummary(filename, FileChooser.convertSize(file.length())));	// update the info TextView
			
		}
	}
	
	public void record() {
		int channelConfiguration = AudioFormat.CHANNEL_IN_MONO;
		int audioEncoding = AudioFormat.ENCODING_PCM_16BIT;
		File file = new File(getCacheDir() + "/temp.pcm");

		// Delete any previous recording.
		if (file.exists())
			file.delete();

		// Create the new file.
		try {
			file.createNewFile();
		} catch (IOException e) {
			throw new IllegalStateException("Failed to create "
					+ file.toString());
		}

		try {
			// Create a DataOuputStream to write the audio data into the saved
			// file.
			OutputStream os = new FileOutputStream(file);
			BufferedOutputStream bos = new BufferedOutputStream(os);
			DataOutputStream dos = new DataOutputStream(bos);

			// Create a new AudioRecord object to record the audio.
			int bufferSize = AudioRecord.getMinBufferSize(SAMPLERATE,
					channelConfiguration, audioEncoding);
			AudioRecord audioRecord = new AudioRecord(
					MediaRecorder.AudioSource.MIC, SAMPLERATE,
					channelConfiguration, audioEncoding, bufferSize);
			
			int bufferSizeShort = bufferSize/2;	// Since getMinBufferSize returns the size in bytes
												// our short buffer size needs to be half of that (short = 2 bytes)
			
			short[] buffer = new short[bufferSizeShort];
			long timestart = System.currentTimeMillis();
			time = 0;
			audioRecord.startRecording();

			while (isRecording) {
				int bufferReadResult = audioRecord.read(buffer, 0, bufferSizeShort);
				for (int i = 0; i < bufferReadResult; i++) {
					dos.writeShort(buffer[i]);
				}
				if (System.currentTimeMillis()-timestart >= 1000) {
					time = time + 1;
					timestart = System.currentTimeMillis();
				}
				runOnUiThread(new Runnable() {
					public void run() {
						status.setText("Recording... " + time + " s.\n Press Stop at any time to stop recording.");
					}
				});
			}
			
			audioRecord.stop();
			dos.close();
			
			audioRecord.release();
			
			runOnUiThread(new Runnable() {
				public void run() {
					status.setText("Finished recording.");
				}
			});			
			updateinfo("(Recorded Audio)", FileChooser.convertSize(file.length()));

		} catch (Throwable t) {
			Log.e("AudioRecord", "Recording Failed");
		}
	}
	
	public void play() {
		Log.i(TAG, "Started play method");
		// Set isPlaying
		isPlaying = true;
		
		runOnUiThread(new Runnable() {
			public void run() {
				status.setText("Processing audio...");
			}
		});
		
		Log.i(TAG, "Get the file from memory");
		// Get the file we want to playback.
		File file = new File(getCacheDir() + "/temp.pcm");
		// Get the length of the audio stored in the file (16 bit so 2 bytes per
		// short)
		// and create a short array to store the recorded audio.
		int musicLength = (int) (file.length() / 2);
		short[] music = new short[musicLength];
		short[] music_fx = new short[musicLength];

		try {
			// Check if file is empty
			if(!file.exists() || SAMPLERATE == 0) throw new FileNotFoundException("File was not loaded");
			
			Log.i(TAG, "Create InputStream");
			// Create a DataInputStream to read the audio data back from the
			// saved file.
			InputStream is = new FileInputStream(file);
			BufferedInputStream bis = new BufferedInputStream(is);
			DataInputStream dis = new DataInputStream(bis);

			Log.i(TAG, "Read the file into the buffer");
			// Read the file into the music array and add effect.
			int i = 0;
			while (dis.available() > 0) {
				music[i] = dis.readShort();
				//music[i] = (short)(music[i] + 0.35*music[Math.max((i-SAMPLERATE/3), 0)]); //Reverb effect
				//music[musicLength - 1 - i] = dis.readShort(); //Reverse effect
				i++;
			}

			// Close the input streams.
			dis.close();
			
			Log.i(TAG, "Add effects to the audio");
			// Add effect created by the JNI code and calculate how long it takes to do this processing
			// By running the app in a Nexus 4 mobile phone, the following elapsed time was obtained:
			// Elapsed time was around 150 ms for a 10 seconds input at 44100Hz sample rate
			long start = System.nanoTime();
			music_fx = addeffects(music, SAMPLERATE, musicLength);
			final double elapsed = Math.round(10000*(double)(System.nanoTime() - start)/1000000.0)/10000.0;
			runOnUiThread(new Runnable() {
				public void run() {
					Toast.makeText(getApplicationContext(), "The C function took " + elapsed + " ms at a sample rate of "
							+ SAMPLERATE + " Hz for a " + time + " seconds recording", Toast.LENGTH_SHORT).show();
				}
			});

			Log.i(TAG, "Create the AudioTrack object");
			// Create a new AudioTrack object using the same parameters as the
			// AudioRecord
			// object used to create the file.
			int channelConfiguration = AudioFormat.CHANNEL_OUT_MONO;
			int audioEncoding = AudioFormat.ENCODING_PCM_16BIT;
			int buffersize = AudioTrack.getMinBufferSize(SAMPLERATE, channelConfiguration, audioEncoding);
			
			AudioTrack audioTrack = new AudioTrack(AudioManager.STREAM_MUSIC,
					SAMPLERATE, channelConfiguration,
					audioEncoding, buffersize,
					AudioTrack.MODE_STREAM);
			
			// Tell the user that the original audio is being played
			runOnUiThread(new Runnable() {
				public void run() {
					status.setText("Playing Original...");
				}
			});
			
			Log.i(TAG, "Starts playing the original track");
			// Start playback of original track
			audioTrack.play();	
			// Write the music buffer to the AudioTrack object
			audioTrack.write(music, 0, musicLength);
			// Stop playback of original track
			audioTrack.stop();
			
			
			// Tell the user that the output audio is being played
			runOnUiThread(new Runnable() {
				public void run() {
					status.setText("Playing FX Version...");
				}
			});

			Log.i(TAG, "Starts playing the fx track");
			// Start playback of original track
			audioTrack.play();	
			// Write the music buffer to the AudioTrack object
			audioTrack.write(music_fx, 0, musicLength);
			// Stop playback of original track
			audioTrack.stop();
			audioTrack.release();
			Log.i(TAG, "Finished play method");

		} catch (FileNotFoundException e) {
			runOnUiThread(new Runnable(){
				public void run(){
					Toast.makeText(getApplicationContext(), 
							"Error: Choose a file or record audio.", Toast.LENGTH_SHORT).show();
				}
			});
		} catch (Throwable t) {
			Log.e("AudioTrack", "Playback Failed : " + t.getMessage());
		} finally { 
			isPlaying = false;
			runOnUiThread(new Runnable() {
				public void run() {
					try {sleep(1000);} catch(Exception e){}
					status.setText("");
				}
			});
		}
	}

	public void takeFFT() {
		// tell the user the current status
		runOnUiThread(new Runnable(){
			public void run(){
				status.setText("Loading the file from Cache...");
			}
		});
		// Get the input to take the FFT and store it in a buffer
		File input_file = new File(getCacheDir() + "/temp.pcm");
		// Get the length of the audio stored in the file (16 bit so 2 bytes per
		// short)
		// and create a double array to store the recorded audio. (similar to MatLab, with the appropriate conversions)
		int audioLength = (int) (input_file.length() / 2);
		double[] audio = new double[audioLength];

		try {
			// Check if file is empty
			if(!input_file.exists() || SAMPLERATE == 0) throw new FileNotFoundException("File was not loaded");
			
			Log.i(TAG, "Create InputStream");
			// Create a DataInputStream to read the audio data back from the
			// saved file.
			InputStream is = new FileInputStream(input_file);
			BufferedInputStream bis = new BufferedInputStream(is);
			DataInputStream dis = new DataInputStream(bis);

			Log.i(TAG, "Read the file into the buffer");
			// Read the file into the audio array, converting to a format similar to MatLab's wavread.
			int i = 0;
			while (dis.available() > 0) {
				audio[i] = (double)dis.readShort()/32768;
				i++;
			}

			// Close the input streams.
			dis.close();
		} catch (FileNotFoundException e) {
			runOnUiThread(new Runnable(){
				public void run(){
					Toast.makeText(getApplicationContext(), 
							"Error: Choose a file or record audio.", Toast.LENGTH_SHORT).show();
					status.setText("");
				}
			});
			return;
		} catch (Throwable t) {
			Log.e("TakeFFT", "Failed to Get Audio : " + t.getMessage());
		}
		
		// Take 60ms FFTs of the audio, shifting by 10ms
		Log.i(TAG, "Starts the FFT variable initialization");
		
			// tell the user the current status
		runOnUiThread(new Runnable(){
			public void run(){
				status.setText("Creating resources...");
			}
		});
		
			// create resources to save results to file
		OutputStream fos;
		PrintWriter pr;
		File dir = new File(Environment.getExternalStorageDirectory() + "/FFT_results");
		
		if (!dir.exists()){
			dir.mkdir();
		}
		File output_file = new File(dir, "/fft.txt");

			// Delete any previous text file.
		if (output_file.exists())
			output_file.delete();

			// Create the new file.
		try {
			output_file.createNewFile();
		} catch (IOException e) {
			Log.e(TAG, "Failed to create fft.txt"); 
		}
		
			// variable creation & initialization
		int point_per_window = (int)Math.round(0.06*SAMPLERATE);       // number of samples per frame
		int point_per_timestep = (int)Math.round(0.01*SAMPLERATE);	// number of samples per time step
		int minframeIndex = 0;
		
			// Log for debugging
		Log.i(TAG, "point_per_window = " + point_per_window);
		Log.i(TAG, "point_per_timestep = " + point_per_timestep);
		Log.i(TAG, "audiolength = " + audioLength);
		
		Log.i(TAG, "Starts performing the FFT");
			// Perform the 60ms FFT in the audio buffer
		try {
			fos = new FileOutputStream(output_file);	// create output stream
			pr = new PrintWriter(fos);					// create print writer
			
			long start = System.nanoTime();		// start time (for elapsed time calculation)
			
			//FFT routine
			FFT_percent = 0;
			final int nfrm_max = (int)Math.floor((double)(audioLength - point_per_window)/(double)point_per_timestep)+1;
			int nfrm_cur = 1;
			double[] frame = new double[point_per_window];
			double[] fft_60ms = new double[65536];
			
			while (nfrm_cur <= nfrm_max) {
				
//				// FOR DEBUGGING ONLY -----
//				Log.i(TAG, "minframeIndex = " + minframeIndex);
//				Log.i(TAG, "maxframeIndex = " + maxframeIndex);
//				Log.i(TAG, "FFT_percent = " + FFT_percent);
//				// -----
				
				// tell the user the current status
				runOnUiThread(new Runnable(){
					public void run(){
						status.setText(String.format("Performing FFT %.2f %%", FFT_percent));
					}
				});
				
				// load the frame buffer with the respective contents of the audio buffer
				for (int i = 0; i < point_per_window; i++) {
					frame[i] = audio[minframeIndex+i];
				}
				
				// take the FFT
				fft_60ms = absfft60ms(frame, SAMPLERATE);
				
				// save the FFT result in the respective line of the text file
				FFTtoTXT(fft_60ms, pr);
				
				// calculate the current percentage
				FFT_percent = (100*(double)(nfrm_cur)/(double)(nfrm_max));
				
				// tell the user the current status
				runOnUiThread(new Runnable(){
					public void run(){
						status.setText(String.format("Performing FFT %.2f %%", FFT_percent));
					}
				});
				
				// update the min and max frameIndex and current frame
				minframeIndex += point_per_timestep;
				nfrm_cur += 1;
			}
			
			final double elapsed = Math.round(100*(double)(System.nanoTime() - start)/1000000000.0)/100.0;
			runOnUiThread(new Runnable() {
				public void run() {
					Toast.makeText(getApplicationContext(), "The C function took " + elapsed + " seconds to take FFT of "
							+ nfrm_max + " samples of audio", Toast.LENGTH_LONG).show();
				}
			});
			
			// close the output streams
			pr.close();
			fos.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		runOnUiThread(new Runnable(){
			public void run(){
				status.setText("Finished FFT.\nThe result is available at SDCARD/FFT_results/fft.txt");
			}
		});
		
		Log.i(TAG, "End of TakeFFT.");

	}
	
//	public void FFTtoTXT(double[] fft, PrintWriter pr) {
//		try {
//			for (int i = 0; i < fft.length; i++) {
//				Log.i(TAG, "Printing " + i + "position to txt file");
//				pr.printf("%1.15f", fft[i]);
//				pr.print(" ");
//			}
//			pr.println("");
//		} catch (Throwable t) {
//			Log.e("FFTtoTXT", "Text file creation Failed");
//		}
//	}
	
	public void FFTtoTXT(double[] fft, PrintWriter pr) {
		try {
			pr.print(Arrays.toString(fft));
			pr.println("");
		} catch (Throwable t) {
			Log.e("FFTtoTXT", "Text file creation Failed");
		}
	}
	
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		// Check if the returned data came from FileChoose (requestCode 1) and if
		// data was actually passed
		if (requestCode == 1 && resultCode == RESULT_OK) {			
			filename = data.getStringExtra("filename");
			filepath = data.getStringExtra("filepath");
			
			if (filename.endsWith(".wav")) {			
			// Call the load() method to convert the .wav file into .pcm
				loadButton.setEnabled(false);
				playButton.setEnabled(false);
				recButton.setEnabled(false);
				fftButton.setEnabled(false);
				Thread loadThread = new Thread(new Runnable() {
					public void run() {
						load();
					}
				});
				loadThread.start();
				
				try{
					loadThread.join();
				} catch (Exception e) { }
				
				loadButton.setEnabled(true);
				playButton.setEnabled(true);
				recButton.setEnabled(true);
				fftButton.setEnabled(true);
			}
			else {
				Toast.makeText(this, "Error: The selected file was not a wav file."
						+ "\nPlease choose another file or record audio.", Toast.LENGTH_SHORT).show();
			}
		}
		else {
			Toast.makeText(this, "Error: No file was selected."
					+ "\nPlease choose a file or record audio.", Toast.LENGTH_SHORT).show();
		}
	}
	
	
	public void spinner_init(){
		List<String> list = new ArrayList<String>();
		list.add("8000");
		list.add("11025");
		list.add("16000");
		list.add("22050");
		list.add("32000");
		list.add("44100");
		ArrayAdapter<String> dataAdapter = new ArrayAdapter<String>(this,
				android.R.layout.simple_spinner_item, list);
			dataAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
			SR.setAdapter(dataAdapter);
	}
	
	
	public void sleep(int i) throws InterruptedException {
		Handler handler = new Handler(); 
	    handler.postDelayed(new Runnable() { 
	         public void run() { } 
	    }, i); 
	}
	
	
	public void updateinfo(final String summary) {
		runOnUiThread(new Runnable() {
			public void run() {
				info.setText(Html.fromHtml(summary));
			}
		});	
	}
	
	
	public void updateinfo(String name, String dataSize) {
		String ByteRate = String.valueOf(SAMPLERATE * 1 * 16/8);	// = SampleRate * NumChannels * BitsPerSample/8
		String BlockAlign = String.valueOf(1 * 16/8);				// = NumChannels * BitsPerSample/8
		String newline = "<br>";
		
		String timelabel;		
		if (time == 1) timelabel = " second";
		else timelabel = " seconds";
		
		final String summary = "<html>Name: " + name + newline + "Duration: " + time + timelabel 
				+ newline + "Format: PCM" + newline + "Channels: 1" + newline + "SampleRate: " + SAMPLERATE + " Hz"
				+ newline + "ByteRate: " + ByteRate + " bytes/second"
				+ newline + "BlockAlign: " + BlockAlign + " bytes/sample"  
				+ newline + "Quantization: " + 16 + " bits/sample"
				+ newline + "DataSize: " + dataSize + "</html>";
		
		runOnUiThread(new Runnable() {
			public void run() {
				info.setText(Html.fromHtml(summary));
			}
		});	
	}
}	
