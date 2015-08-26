// ScreenRecorder.h

#pragma once

#include "Stdafx.h"
#include <wmencode.h>
#include <Atlbase.h>
#include <comdef.h>

namespace Inkostar
{
	namespace Assemblies
	{
		class WmEncoder
		{
			#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
															((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
															((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))			//From MMSystem.h

			#define MAX_LOADSTRING 100

			#define	WM_NOTIFYICON_MESSAGE	WM_USER+1000

			#define CONTENT_TYPE_VIDEO_ONLY		16		//Profile Supports Only Video
			#define CONTENT_TYPE_AUDIO_VIDEO	17		//Profile Supports both audio and Video

			#define PROFILE_CONTENT_TYPE		CONTENT_TYPE_AUDIO_VIDEO		//Change this to Audio_Video for Audio support

			#define PROFILE_AUDIENCE_BITRATE	400000	//Target for 400 Kbps

			#define VIDEOCODEC					MAKEFOURCC('M','S','S','2')

			#define VIDEOFRAMERATE				10		//15 FPS(Frames Per Second)

			#define IMAGEQUALITY				95		// 0 ~ 100: The higher the number the better the quality

			#define CAPTURE_FULLSCREEN			true	//By Default We capture Full Screen - If false set the coordinates

			#define	WMSRCNCAP_CAPTUREWINDOW		CComBSTR("CaptureWindow")	//Property of Source Video - Fullscreen Mode

			#define WMSCRNCAP_WINDOWLEFT        CComBSTR("Left")
			#define WMSCRNCAP_WINDOWTOP         CComBSTR("Top")
			#define WMSCRNCAP_WINDOWRIGHT       CComBSTR("Right")
			#define WMSCRNCAP_WINDOWBOTTOM      CComBSTR("Bottom")
			#define WMSCRNCAP_FLASHRECT         CComBSTR("FlashRect")
			#define WMSCRNCAP_ENTIRESCREEN      CComBSTR("Screen")
			#define WMSCRNCAP_WINDOWTITLE       CComBSTR("WindowTitle")

			#define ErrorMessage(x)	MessageBox(NULL,x,"Error",MB_OK|MB_ICONERROR)					

		public:
			IWMEncoder2*		g_pEncoder;
			IWMEncProfile2*		g_pProfile;

			WmEncoder()
			{					
				HRESULT hr = CoInitialize(NULL);					//Initialize COM								
				hr = CoInitialize(NULL);
				hr = CoInitialize(NULL);
				hr = CoInitialize(NULL);

				if (hr == S_FALSE)
				{
					ErrorMessage("CoInitializeEx failed!");
				}
				g_pEncoder = NULL;
				g_pProfile = NULL;
			}

			HRESULT	SetupScreenCaptureProfile()										//Called by InitEncoder() Function
			{
				IWMEncAudienceObj*	pAudience = NULL;

				if (FAILED(CoCreateInstance(CLSID_WMEncProfile2, NULL, CLSCTX_INPROC_SERVER, IID_IWMEncProfile2, (void**)&g_pProfile)))
				{
					ErrorMessage("Unable to Create Profile Manager");
					return E_FAIL;
				}
				if (FAILED(g_pProfile->put_ValidateMode(true)))						//Verify the settings immediately as they are set
				{
					ErrorMessage("Unable to Set Validate Mode");
					return E_FAIL;
				}
				if (FAILED(g_pProfile->put_ProfileName(CComBSTR("Custom Screen Capture Profile"))))
				{
					ErrorMessage("Unable to Set Profile Name");
					return E_FAIL;
				}
				if (FAILED(g_pProfile->put_ProfileDescription(CComBSTR("A Custom Video Profile For Screen Capture"))))
				{
					ErrorMessage("Unable to Set Profile Description");
					return E_FAIL;
				}
				if (FAILED(g_pProfile->put_ContentType(PROFILE_CONTENT_TYPE)))		//Content Type for our Profile - By default it is - CONTENT_TYPE_VIDEO_ONLY
				{
					ErrorMessage("Unable to Set Content Type for Profile");
					return E_FAIL;
				}
				if (FAILED(g_pProfile->put_VBRMode(WMENC_VIDEO, 0, WMENC_PVM_NONE)))	//Set CBR Mode - Compatible with Older Version Players
				{
					ErrorMessage("Unable to Set BitRate Mode");
					return E_FAIL;
				}
				if (FAILED(g_pProfile->AddAudience(PROFILE_AUDIENCE_BITRATE, &pAudience)))
				{
					ErrorMessage("Unable to Set Target Audience");
					return E_FAIL;
				}

				do
				{
					long lCodecIndex = -1;
					if (FAILED(g_pProfile->GetCodecIndexFromFourCC(WMENC_VIDEO, VIDEOCODEC, &lCodecIndex)))
					{
						ErrorMessage("Unable to Get Codec Index");
						break;
					}
					if (FAILED(pAudience->put_VideoCodec(0, lCodecIndex)))			//Set the Codec for the Target Audience
					{
						ErrorMessage("Unable to Set Codec");
						break;
					}
					if (FAILED(pAudience->put_VideoHeight(0, 0)))						//Use the same height as the input
					{
						ErrorMessage("Unable to Set Video Height");
						break;
					}
					if (FAILED(pAudience->put_VideoWidth(0, 0)))						//Use the same width as the input
					{
						ErrorMessage("Unable to Set Video Width");
						break;
					}
					if (FAILED(pAudience->put_VideoBufferSize(0, 5000)))				//Set buffer size for 5 seconds
					{
						ErrorMessage("Unable to Set Buffer Size");
						break;
					}
					if (FAILED(pAudience->put_VideoFPS(0, VIDEOFRAMERATE * 1000)))	//Set the Video Frame Rate
					{
						ErrorMessage("Unable to Set Video Frame Rate");
						break;
					}
					if (FAILED(pAudience->put_VideoImageSharpness(0, IMAGEQUALITY)))	//Set the Best Image Sharpness possible
					{
						ErrorMessage("Unable to Set Video Image Sharpness");
						break;
					}
					if (FAILED(g_pProfile->Validate()))								//Test the Profile Settings
					{
						ErrorMessage("Unable to Validate the Profile Settings");
						break;
					}
					if (pAudience)
					{
						pAudience->Release();
						pAudience = NULL;
					}
					return S_OK;

				} while (false);

				if (pAudience)						//Control Reaches here only in case of errors 
				{
					pAudience->Release();
					pAudience = NULL;
				}
				return E_FAIL;
			}

			HRESULT	InitEncoder(LPCTSTR szOutputFileName)
			{
				HRESULT	hr = E_FAIL;
				CComVariant varValue;
				IWMEncSourceGroupCollection*	pSrcGrpCollection = NULL;
				IWMEncSourceGroup*	pSrcGrp = NULL;
				IWMEncSource*	pSrc = NULL;
				IPropertyBag*	pPropertyBag = NULL;
				IWMEncVideoSource2*	pSrcVid = NULL;
				IWMEncFile*	pOutFile = NULL;
				IWMEncProfile*	pProfile = NULL;

				hr = CoCreateInstance(CLSID_WMEncoder, NULL, CLSCTX_INPROC_SERVER, IID_IWMEncoder2, (void**)g_pEncoder);

				if (FAILED(hr))
				{
					DWORD err = GetLastError();

					ErrorMessage("Unable to Create Encoder Object");
					return E_FAIL;
				}
				if (FAILED(g_pEncoder->get_SourceGroupCollection(&pSrcGrpCollection)))		//Retrieve the Source Group Collection	- One Application can Have many Source Groups - We need to add as many as we want
				{
					ErrorMessage("Unable to Get Source Group Collection");
					return E_FAIL;
				}

				do
				{
					if (FAILED(hr = pSrcGrpCollection->Add(CComBSTR("SourceGroup1"), &pSrcGrp)))//Add a Source Group to the Collection - Each Source can have one video one audio source input
					{
						ErrorMessage("Unable to Add A Source Group to the Collection");
						break;
					}
					if (FAILED(hr = pSrcGrp->AddSource(WMENC_VIDEO, &pSrc)))					//Add a Video Source to the Group
					{
						ErrorMessage("Unable to Add A Source to the Source Group");
						break;
					}
					if (FAILED(hr = pSrc->QueryInterface(IID_IWMEncVideoSource2, (void**)&pSrcVid)))
					{
						ErrorMessage("Unable to Query interface for Video Source");
						break;
					}
					if (FAILED(hr = pSrcVid->SetInput(CComBSTR("ScreenCap://ScreenCapture1"))))//The Video Input Source Device - Should be "ScreenCap" Device
					{
						ErrorMessage("Unable to Set Video Input Source");
						break;
					}
					if (FAILED(hr = pSrcVid->QueryInterface(IID_IPropertyBag, (void**)&pPropertyBag)))
					{
						ErrorMessage("Unable to Query Interface for Propery bag");
						break;
					}

					varValue = CAPTURE_FULLSCREEN;
					if (FAILED(hr = pPropertyBag->Write(WMSCRNCAP_ENTIRESCREEN, &varValue)))	//Set Full Screen Property true/false
					{
						ErrorMessage("Unable to Set Capture Screen Property");
						break;
					}
					if (FAILED(hr = SetupScreenCaptureProfile()))									//Setup the Custom Profile
					{
						break;
					}
					if (FAILED(hr = g_pProfile->QueryInterface(IID_IWMEncProfile, (void**)&pProfile)))
					{
						ErrorMessage("Unable to Query Interface For Profile");
						break;
					}
					if (FAILED(hr = pSrcGrp->put_Profile(variant_t(pProfile))))					//Select the Custom Profile into the Encoder	
					{
						ErrorMessage("Unable to Set Profile For Source Group");
						break;
					}
					if (FAILED(hr = g_pEncoder->get_File(&pOutFile)))
					{
						ErrorMessage("Unable to Get Encoder Output File Object");
						break;
					}
					if (FAILED(hr = pOutFile->put_LocalFileName(CComBSTR(szOutputFileName))))		//Set the Target Output Filename
					{
						ErrorMessage("Unable to Set Output File Name");
						break;
					}
					if (FAILED(hr = g_pEncoder->PrepareToEncode(VARIANT_TRUE)))					//Using Prepare optimizes startig latency
					{
						ErrorMessage("Unable to Prepare for Encoding");
						break;
					}
				} while (false);

				if (pProfile)
				{
					pProfile->Release();
					pProfile = NULL;
				}
				if (pOutFile)
				{
					pOutFile->Release();
					pOutFile = NULL;
				}
				if (pPropertyBag)
				{
					pPropertyBag->Release();
					pPropertyBag = NULL;
				}
				if (pSrcVid)
				{
					pSrcVid->Release();
					pSrcVid = NULL;
				}
				if (pSrc)
				{
					pSrc->Release();
					pSrc = NULL;
				}
				if (pSrcGrp)
				{
					pSrcGrp->Release();
					pSrcGrp = NULL;
				}
				if (pSrcGrpCollection)
				{
					pSrcGrpCollection->Release();
					pSrcGrpCollection = NULL;
				}
				return hr;
			}

			void Cleanup()
			{
				if (g_pProfile)
				{
					g_pProfile->Release();
					g_pProfile = NULL;
				}
				if (g_pEncoder)
				{
					g_pEncoder->PrepareToEncode(VARIANT_FALSE);		//Prepare to Stop
					g_pEncoder->Stop();
					g_pEncoder->Release();
					g_pEncoder = NULL;
				}

				CoUninitialize();
			}

			//Class Destructor
			~WmEncoder()
			{
				Cleanup();
			}			
		};
	}
}
