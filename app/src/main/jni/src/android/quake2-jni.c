/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0 
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <jni.h>
#include <android/log.h>

#include "../ref_soft/r_local.h"
//#include "rw_android.h"
#include "quake2-jni.h"
//#include <GLES/gl.h>
//#include "gl.h"

#define QUAKE2_JNI_VERSION "1.8"


#define EXPORT_ME __attribute__ ((visibility("default")))
//#define EXPORT_ME __attribute__ ((dllexport))
//#define EXPORT_ME __declspec(dllexport)


jstring EXPORT_ME
Java_com_wattz_dexq2_Quake2_Quake2GetVersion( JNIEnv* env,
                                                  jobject thiz )
{
    return (*env)->NewStringUTF(env, QUAKE2_JNI_VERSION);
}

static int 	oldtime ;

extern int Sys_Milliseconds(void);
extern void Qcommon_Init(int argc, char **argv);
extern void Qcommon_Frame(int msec);
//extern void* Cvar_Set (char *var_name, char *value);




jint EXPORT_ME
Java_com_wattz_dexq2_Quake2_Quake2Init( JNIEnv* env,
                                                  jobject thiz )
{
	int argc =1; char *argv[] ={ "quake2" };

	/// check for static init problem (see below)
	static int init_state = 0;
	if (init_state!=0)
		return 1;
	init_state = 1;

	//int argc =4; char *argv[] ={ "quake2", "+set", "developer", "1" };








    __android_log_print(ANDROID_LOG_ERROR, "quake2-jni.c", "native code : Quake2Init enter\n");
    __android_log_print(ANDROID_LOG_INFO, "quake2-jni.c", "Quake 2 JNI -- Version %s\n", QUAKE2_JNI_VERSION);
    
    /* HAVE_NEON is defined in Android.mk ! */
#ifdef HAVE_NEON
/*
    if ((features & ANDROID_CPU_ARM_FEATURE_NEON) == 0) {
        strlcat(buffer, "CPU doesn't support NEON !\n", sizeof buffer);
        goto EXIT;
    }*/
    __android_log_print(ANDROID_LOG_INFO, "quake2-jni.c", "Program compiled with ARMv7 support !\n");
#else /* !HAVE_NEON */
    __android_log_print(ANDROID_LOG_INFO, "quake2-jni.c", "Program *NOT* compiled with ARMv7 support !\n");
#endif /* !HAVE_NEON */
   
    
	//Cvar_Set("developer","1");


	Qcommon_Init(argc, argv);


	oldtime = Sys_Milliseconds ();

	__android_log_print(ANDROID_LOG_DEBUG, "quake2-jni.c", "native code : Quake2Init exit\n");

	return 0;
}



	/*
	 *
	 * static data warning :
	 *  on Android, shared lib are not reloaded,
	 *  => static data stay in the same state

	**** first time ****
	D/dalvikvm(  715): Trying to load lib /data/data/com.example.quake2/lib/libquake2.so 0x499401b0
	I/ActivityManager(  582): Displayed activity com.example.quake2/.Quake2: 1905 ms
	D/dalvikvm(  715): Added shared lib /data/data/com.example.quake2/lib/libquake2.so 0x499401b0
	D/dalvikvm(  715): No JNI_OnLoad found in /data/data/com.example.quake2/lib/libquake2.so 0x499401b0
	D/dalvikvm(  715): +++ not scanning '/system/lib/libwebcore.so' for 'getVersion' (wrong CL)
	I/Quake2  (  715): version : 1.0
	D/dalvikvm(  715): +++ not scanning '/system/lib/libwebcore.so' for 'Quake2Test' (wrong CL)
	D/quake2-jni.c(  715): init_done= 0
	I/ARMAssembler(  715): generated scanline__00000077:03010144_00000000_00000000 [ 11 ipp] (45 ins) at [0x198c48:0x198cfc] in 2648382 ns
	D/quake2-jni.c(  715): init_done= 1

	**** second time ****
	D/dalvikvm(  956): Trying to load lib /data/data/com.example.quake2/lib/libquake2.so 0x49b36f60
	D/dalvikvm(  956): Shared lib '/data/data/com.example.quake2/lib/libquake2.so' already loaded in same CL 0x49b36f60
	I/Quake2  (  956): version : 1.0
	D/quake2-jni.c(  956): init_done= 1
	I/ActivityManager(  587): Displayed activity com.example.quake2/.Quake2: 1253 ms
	D/quake2-jni.c(  956): init_done= 1

	*/

extern int menu_is_bind_grabbed();
jint EXPORT_ME
Java_com_wattz_dexq2_Quake2_Quake2PointerGrabState( JNIEnv* env, jobject thiz ) {
	/* Dirty Hack - need to get static object bind_grab from client/menu.c
	   requires modifcation of client/menu.c which I'd rather not do but there is no clean 
	   way to get that value otherwise
	   
	   To mod the Q2 sources again if needed replace line
	   static int		bind_grab;
	   with
	   int menu_is_bind_grabbed() {
	       return bind_grab;
	   }
	*/
	return pointer_grab_state || menu_is_bind_grabbed();
}
void IN_ActivateMouse() {
	pointer_grab_state = 1;
}
void IN_DeactivateMouse() {
	pointer_grab_state = 0; 
}


jint EXPORT_ME
Java_com_wattz_dexq2_Quake2_Quake2Frame( JNIEnv* env,
                                                  jobject thiz )
{

	int 	time, newtime;


	//__android_log_print(ANDROID_LOG_DEBUG, "quake2-jni.c", "native code : Quake2Frame enter\n");


	quake2_jni_reset_framecount();

	// find time spent rendering last frame
	do {
		newtime = Sys_Milliseconds ();
		time = newtime - oldtime;
	} while (time < 1);

	Qcommon_Frame (time);

	oldtime = newtime;



	//__android_log_print(ANDROID_LOG_DEBUG, "quake2-jni.c", "native code : Quake2Frame exit\n");

    return quake2_jni_get_framecount();
}



//Cvar_Set ("paused", "1")

jint EXPORT_ME
Java_com_wattz_dexq2_Quake2_Quake2Paused( JNIEnv* env,
                                                  jobject thiz )
{
    // from qcommon.h
//    extern cvar_t *Cvar_Get (char *var_name, char *value, int flags);

    // from cl_main.c
/*
    cvar_t	*cl_paused;

	cl_paused = Cvar_Get ("paused", "0", 0);
*/
    extern cvar_t	*cl_paused;

// from q_shared.c
/*
// nothing outside the Cvar_*() functions should modify these fields!
typedef struct cvar_s
{
	char		*name;
	char		*string;
	char		*latched_string;	// for CVAR_LATCH vars
	int			flags;
	qboolean	modified;	// set each time the cvar is changed
	float		value;
	struct cvar_s *next;
} cvar_t;
*/

	return  (cl_paused->value ? 1 : 0);
}


jint EXPORT_ME
Java_com_wattz_dexq2_Quake2_Quake2Quit( JNIEnv* env,
                                                  jobject thiz )
{
	//Sys_Quit();
	CL_Shutdown ();
	Qcommon_Shutdown ();
	//fcntl (0, F_SETFL, fcntl (0, F_GETFL, 0) & ~FNDELAY);
	// 	_exit(0);
	return 0;
}


jint EXPORT_ME
Java_com_wattz_dexq2_Quake2_Quake2Test( JNIEnv* env,
                                                  jobject thiz )
{
	return 0;
}



jint EXPORT_ME
Java_com_wattz_dexq2_Quake2_Quake2GetDisableScreen( JNIEnv* env,
                                                  jobject thiz )
{
	extern int quake2_jni_disable_screen();
	return quake2_jni_disable_screen();
}


jint EXPORT_ME
Java_com_wattz_dexq2_Quake2_Quake2GetVibration( JNIEnv* env,
                                                  jobject thiz )
{
	extern int quake2_jni_get_vibration();
	return quake2_jni_get_vibration();
}

///////////////////////////

static int width;
static int height;


void EXPORT_ME
Java_com_wattz_dexq2_Quake2_Quake2SetWidth( JNIEnv* env,
                                                  jobject thiz, jint value )
{
	width = value;
}

void EXPORT_ME
Java_com_wattz_dexq2_Quake2_Quake2SetHeight( JNIEnv* env,
                                                  jobject thiz, jint value )
{
	height = value;
}

int quake2_jni_get_width()
{
	return width;
}

int quake2_jni_get_height()
{
	return height;
}


void EXPORT_ME
Java_com_wattz_dexq2_Quake2_Quake2SetOverlay( JNIEnv* env,
                                                  jobject thiz, jint value )
{
	extern void quake2_jni_set_overlay( int value );
	quake2_jni_set_overlay( value );
}


/*************************
 * I/O
 *************************/




void EXPORT_ME
Java_com_wattz_dexq2_Quake2_Quake2KeyEvent(JNIEnv* env, jobject obj,
			jint key, jint down) {

	quake2_jni_key_event( key, down );


}




void EXPORT_ME
Java_com_wattz_dexq2_Quake2_Quake2MoveEvent(JNIEnv* env, jobject obj,
			jint mode,
			jint forwardmove, jint sidemove, jint upmove,
			jfloat pitch, jfloat yaw, jfloat roll ) {

	/*
	__android_log_print(ANDROID_LOG_DEBUG, "quake2-jni.c",
			"Quake2MoveEvent forwardmove= %d yaw= %.0f\n", forwardmove, yaw);
*/

	quake2_jni_move_event( mode,
			forwardmove,  sidemove,  upmove,
			pitch, yaw, roll
			);

}








/*************************
 * Audio stuff
 *************************/

jint EXPORT_ME
Java_com_wattz_dexq2_Quake2_Quake2PaintAudio( JNIEnv* env,
                                                  jobject thiz, jobject buf )
{
	extern int paint_audio (void *unused, void * stream, int len);

	void *stream;
	int len;


	stream = (*env)->GetDirectBufferAddress( env,  buf);
	len = (*env)->GetDirectBufferCapacity( env,  buf);

	//__android_log_print(ANDROID_LOG_DEBUG, "quake2-jni.c", "paint_audio %p %d\n", stream, len );

	return paint_audio ( NULL, stream, len );

}


