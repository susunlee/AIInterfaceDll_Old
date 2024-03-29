﻿// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 클래스입니다.
#include <afxodlgs.h>       // MFC OLE 대화 상자 클래스입니다.
#include <afxdisp.h>        // MFC 자동화 클래스입니다.
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC 데이터베이스 클래스입니다.
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO 데이터베이스 클래스입니다.
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

/////////////////////////////////////////////////////////////////////////////////////////////

#include <atlimage.h>
//#include "verify_ai.h"

#ifdef _x64
//#pragma comment (lib, "D:\\My_Project\\검사장비\\AIInterfaceDll\\x64\\Verify_AI.lib")
#pragma comment (lib, ".\\AI_Lib\\x64\\Verify_AI.lib")
#else
#pragma comment (lib, ".\\AI_Lib\\x86\\Verify_AI.lib")
#endif


#define	IMAGE_WIDTH					200
#define	IMAGE_HEIGHT				200
#define IMAGE_ITEMS_PER_LINE		10

#define WIDTHBYTES(bits)			(((bits)+31)/32*4)  // 4바이트 배수

//extern "C" __declspec(dllexport) int Sum(int, int);

// 리턴값 : -1 에러(처리를 못함)
//        :  0 NG이미지
//		  :  1 OK이미지
extern "C" __declspec(dllexport) int AI_FOR_VERIFY_201909(	unsigned char *master_raw,    // master raw 포인터(200*600)
															unsigned char *defect_raw);   // defect raw 포인터(200*600)