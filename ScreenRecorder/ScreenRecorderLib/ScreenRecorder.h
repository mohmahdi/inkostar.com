// ScreenRecorder.h

#pragma once

#include "Stdafx.h"
#include "WmEncoder.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

namespace Inkostar 
{
	namespace Assemblies 
	{		
		public ref class ScreenRecorder
		{		
		public:
			ScreenRecorder(String^ recordingFile)
			{									
				wmEncoder = new WmEncoder();				
				this->recordingFile = recordingFile;								
			}
			
			void Start()
			{							
				IntPtr fil = Marshal::StringToHGlobalAnsi(this->recordingFile);
								
				if (wmEncoder->g_pEncoder)
				{
					WMENC_ENCODER_STATE	encState = WMENC_ENCODER_STOPPED;	//By Default Assume Stopped state
					wmEncoder->g_pEncoder->get_RunState(&encState);
					
					if (encState == WMENC_ENCODER_RUNNING)
					{
						wmEncoder->Cleanup();
					}
				}
				
				if (!FAILED(wmEncoder->InitEncoder((LPCTSTR)fil.ToPointer())))
				{	
					if (FAILED(wmEncoder->g_pEncoder->Start()))
					{
						ErrorMessage("Unable to Start Encoding Process");
					}
				}

				Marshal::FreeHGlobal(IntPtr(fil));
			}

			void Pause()
			{
				if (wmEncoder->g_pEncoder)
				{
					WMENC_ENCODER_STATE	encState = WMENC_ENCODER_STOPPED;	//By Default Assume Stopped state
					wmEncoder->g_pEncoder->get_RunState(&encState);

					if (encState == WMENC_ENCODER_RUNNING)
					{
						if (FAILED(wmEncoder->g_pEncoder->Pause()))
						{
							ErrorMessage("Unable to Start Encoding Process");
						}
					}
				}
			}

			void Shutdown()
			{
				delete(wmEncoder);
				wmEncoder = NULL;
			}

			//Class Destructor
			~ScreenRecorder()
			{
				// Call the finalizer
				this->!ScreenRecorder();
			}

			//Class Finalizer
			!ScreenRecorder()
			{				
				//GC will take care of managed resources
			}

		private:			
			String^ recordingFile;						
			WmEncoder* wmEncoder;
						
		};		
	}
}
