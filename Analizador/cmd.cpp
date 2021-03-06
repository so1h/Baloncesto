//---------------------------------------------------------------------------

#pragma hdrstop

#include "cmd.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#include <windows.h>

long SetScreenResolution ( int width, int height )
{
	DEVMODE DeviceMode ;
	DeviceMode.dmSize = sizeof(DEVMODE) ;
	DeviceMode.dmPelsWidth = width ;
	DeviceMode.dmPelsHeight = height ;
	DeviceMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT ;
	return ChangeDisplaySettings(&DeviceMode, CDS_UPDATEREGISTRY) ;
}

int OldWidth ;
int OldHeight ;

bool CrearCmdWait ( wchar_t * cmd,
					wchar_t * dir,
					wchar_t * env,
					wchar_t * titulo,
					bool      visible,
					int       width,
					int       height,
					bool      esperarTerminacion )
{
	STARTUPINFO StartUp ;
	PROCESS_INFORMATION ProcInfo ;
	bool res ;
//	memset(&StartUp, 0, sizeof(StartUp)) ;
//	StartUp.cb = sizeof(StartUp) ;
    GetStartupInfo(&StartUp) ;
	memset(&ProcInfo, 0, sizeof(ProcInfo)) ;
	if (titulo[0] != '\0')
		StartUp.lpTitle = titulo ;
	if (!visible)
		StartUp.wShowWindow = SW_HIDE ;
    OldWidth = GetSystemMetrics(SM_CXSCREEN) ;
    if ((width = 800) && (width != OldWidth)) 
	{	
        OldHeight = GetSystemMetrics(SM_CYSCREEN) ;
        SetScreenResolution(width, height) ;
    }
	
//	res = CreateProcessA(NULL, /* no hay manera de que funcione */ 
//	res = CreateProcess(NULL,  
	res = CreateProcessW(NULL,  
 						cmd,
						NULL,
						NULL,
						true,
						0,
						NULL,
						dir,
						&StartUp,
						&ProcInfo
    ) ;
	if (esperarTerminacion)
		WaitForSingleObject(ProcInfo.hProcess, INFINITE) ;
	if ((width == 800) && (width != OldWidth))
	{
		SetScreenResolution(OldWidth, OldHeight) ;
	}
	return res ;
}

bool CrearCmd ( wchar_t * cmd,
				wchar_t * dir,
				wchar_t * env,
				wchar_t * titulo,
				bool      visible,
				int       width,
				int       height )
{
    return CrearCmdWait(cmd, dir, env, titulo, visible, width, height, false) ;
}


/* ------------------------------------------------------------------------*/
/* void obtener_ejecutable_firefox ( )                                     */
/* ------------------------------------------------------------------------*/

AnsiString strApp_Firefox = "" ;  /* camino del f. ejecutable de Firefox, o "" */

//	AnsiString strApp_Firefox = "C:\\Program Files\\Mozilla Firefox\\Firefox.exe" ;
//	AnsiString strApp_Firefox = "C:\\Program Files\\Internet Explorer\\iexplore.exe" ;

void obtener_ejecutable_firefox ( void )
{

firefox:

	strApp_Firefox = "C:\\Program Files\\Mozilla Firefox\\Firefox.exe" ;
	if (FileExists(strApp_Firefox))
		return ;
	
//	system("@ftype | @findstr Firecfox > mozilla.firefox") ;               /* findstr /? */
	system("@ftype | @findstr /R \"\^Firefox\" > mozilla.firefox") ;       /* findstr /? */
	
	int df ;
	if ((df = FileOpen("mozilla.firefox", fmOpenRead | fmShareDenyNone)) < 0)
	{
		Application->MessageBox(
			L"El fichero mozilla.firefox no puede abrirse para lectura",
			L"Error de apertura"
		) ;
		FileClose(df) ;
		goto iexplore ;
	}
	
	strApp_Firefox = "" ;
	int i ;
	char car, carInicial ;
	while ((FileRead(df, &carInicial, 1) == 1) && (carInicial != '\"')) { } ;
	if (carInicial == '\"')
	{
		for ( i = 0 ; i < 256 ; i++ )
			if ((FileRead(df, &car, 1) != 1) || (car == '\"'))
				break ;
			else
				strApp_Firefox = strApp_Firefox + car ;
	}
	FileClose(df) ;
	if (FileExists(strApp_Firefox)) return ;
	
iexplore:

//	system("@ftype | @findstr htmlfile > mozilla.firefox") ;               /* findstr /? */
	system("@ftype | @findstr /R \"\^htmlfile=\" > mozilla.firefox") ;     /* findstr /? */

	if ((df = FileOpen("mozilla.firefox", fmOpenRead | fmShareDenyNone)) < 0)
	{
		Application->MessageBox(
			L"El fichero mozilla.firefox no puede abrirse para lectura",
			L"Error de apertura"
		) ;
		FileClose(df) ;
		goto shell ;
	}
	
	strApp_Firefox = "" ;
	while ((FileRead(df, &carInicial, 1) == 1) && (carInicial != '\"')) { } ;
	if (carInicial == '\"')
	{
		for ( i = 0 ; i < 256 ; i++ )
			if ((FileRead(df, &car, 1) != 1) || (car == '\"'))
				break ;
			else
				strApp_Firefox = strApp_Firefox + car ;
	}
	FileClose(df) ;
	if (FileExists(strApp_Firefox)) return ;
	
shell:
    strApp_Firefox = "shell" ;	
}

/* ------------------------------------------------------------------------*/
/* void mostrar_enlace ( enlace )                                          */
/* ------------------------------------------------------------------------*/

/* mostrar_enlace crea un fichero de texto "enlace.URL" del tipo:          */
/*                                                                         */
/* [InternetShortcut]                                                      */
/* URL=http://www.minix3.org                                               */
/*                                                                         */
/* eso en el caso de que enlace sea "http://www.minix3.org"                */ 

void mostrar_enlace ( AnsiString enlace ) 
{
	
	if (strApp_Firefox == "")
		obtener_ejecutable_firefox() ;

	AnsiString strApp = strApp_Firefox ;
	
	if (strApp != "shell") 
	{
    	strApp = "\"" + strApp + "\"" ; /* hace falta directorios con espacios */
		strApp = strApp + " " ;
		wchar_t wstrApp [ 256 ] ;
		StrPCopy(wstrApp, strApp + enlace) ;
		if (CrearCmdWait(
				wstrApp,
				L".", L"", L"hola", true, 1024, 768, false
			)
		   )		
        return ;		
    }
	int dfEnlace ;
   	RawByteString strBytes ;              /* muy importante: RawByteString */
	if ((dfEnlace = FileCreate("enlace.URL")) >= 0)
	{
	    strBytes = 
		    "[InternetShortcut]\n"
            "URL=" + AnsiToUtf8(enlace) ;
        FileWrite(dfEnlace, &strBytes[1], strBytes.Length()) ;		
		FileClose(dfEnlace) ;
//		system("start /B enlace.URL") ;
//      CrearCmd(L"cmd.exe /C start /MIN enlace.URL", L".", L"", L"", false, 1024, 768, false) ;
		system("start /MIN enlace.URL") ;
    }		
}	

/* ------------------------------------------------------------------------*/
/* borrar_enlace ( enlace )                                                */
/* ------------------------------------------------------------------------*/

void borrar_enlace ( void ) 
{
	if (FileExists("enlace.URL")) DeleteFile("enlace.URL") ;
}


/* ------------------------------------------------------------------------*/
/* void obtener_ejecutable_excel ( )                                       */
/* ------------------------------------------------------------------------*/

AnsiString strApp_Excel = "" ;  /* camino del f. ejecutable de excel, o "" */

//	AnsiString strApp_Excel = "C:\\Program Files\\Microsoft Office\\Office16\\EXCEL.exe" ;

void obtener_ejecutable_excel ( void )
{

	int j ;
	for ( j = 19 ; j >= 10 ; j-- ) 
	{
    	strApp_Excel = "C:\\Program Files\\Microsoft Office\\Office" + IntToStr(j) + "\\EXCEL.exe" ;
	    if (FileExists(strApp_Excel))
			return ;
	}
	
//	system("@ftype | @findstr Excel.CSV > excel.csv") ;                 /* findstr /? */
	system("@ftype | @findstr /R \"\^Excel\\.CSV=\" > excel.csv") ;      /* findstr /? */
	int df ;
	if ((df = FileOpen("excel.csv", fmOpenRead | fmShareDenyNone)) < 0)
	{
		Application->MessageBox(
			L"El fichero excel.csv no puede abrirse para lectura",
			L"Error de apertura"
		) ;
		FileClose(df) ;
		return ;
	}
	FileSeek(df, 10, SEEK_SET) ;

	strApp_Excel = "" ;
	int i ;
	char car, carInicial ;
	FileRead(df, &carInicial, 1) ;
	if (carInicial == '\"')
	{
		for ( i = 0 ; i < 256 ; i++ )
			if ((FileRead(df, &car, 1) != 1) || (car == '\"'))
				break ;
			else
				strApp_Excel = strApp_Excel + car ;
	}
	FileClose(df) ;

	if (!FileExists(strApp_Excel)) strApp_Excel = "shell" ;

}


/* ------------------------------------------------------------------------*/
/* void mostrar_fichero_excel ( )                                          */
/* ------------------------------------------------------------------------*/

void mostrar_fichero_excel ( AnsiString FileName )
{

	if (strApp_Excel == "")
		obtener_ejecutable_excel() ;

	AnsiString strApp = strApp_Excel ;
	
	if (strApp != "shell")
	{
		FileName = "\"" + FileName + "\"" ; /* hace falta directorios con espacios */
    	strApp = "\"" + strApp + "\"" ; /* hace falta directorios con espacios */
		strApp = strApp + " " ;
		wchar_t wstrApp [ 256 ] ;
#if 0
//	    StrCopy(wstrApp, L"\"C:\\Program Files\\Microsoft Office\\Office16\\EXCEL.exe\" ") ;
		StrPCopy(wstrApp, strApp) ;
		wchar_t wstrArg [ 256 ] ;
		StrPCopy(wstrArg, FileName) ;
		StrCat(wstrApp, wstrArg) ;
#endif
		StrPCopy(wstrApp, strApp + FileName) ;
		if (CrearCmdWait(
				wstrApp,
				L".", L"", L"hola", true, 1024, 768, false
			)
		   )
		return ;
	}  
//	system("start Y:\\log_e9.crg.csv") ;
	AnsiString startCronograma = "start " + FileName ;
	char strCmd [ 256 ] ;
	StrPCopy((char *)&strCmd, startCronograma) ;
	system(strCmd) ;
//	system("start cmd.exe") ;	
	return ;
}	


/* ------------------------------------------------------------------------*/
/* void obtener_ejecutable_chm ( )                                         */
/* ------------------------------------------------------------------------*/

AnsiString strApp_Chm = "" ;  /* camino del f. ejecutable para abrir .chm, o "" */

//	AnsiString strApp_Excel = "C:\\WINDOWS\\hh.exe" ;

void obtener_ejecutable_chm ( void ) 
{ 
	AnsiString SystemRoot = GetEnvironmentVariable("SystemRoot") ;
	if (FileExists(SystemRoot + "\\hh.exe"))
		strApp_Chm = SystemRoot + "\\hh.exe" ;
	else 
		strApp_Chm = "shell" ;
}

/* ------------------------------------------------------------------------*/
/* void mostrar_fichero_chm ( FileName )                                   */
/* ------------------------------------------------------------------------*/
				
void mostrar_fichero_chm ( AnsiString FileName ) 
{
	if (strApp_Chm == "") 
		obtener_ejecutable_chm() ;

	AnsiString strApp = strApp_Chm ;

	if (strApp != "shell")
	{
		FileName = "\"" + FileName + "\"" ; /* hace falta directorios con espacios */
    	strApp = "\"" + strApp + "\"" ; /* hace falta directorios con espacios */
		strApp = strApp + " " ;
		wchar_t wstrApp [ 256 ] ;
#if 0
//	    StrCopy(wstrApp, L"\"C:\\Program Files\\Microsoft Office\\Office16\\EXCEL.exe\" ") ;
		StrPCopy(wstrApp, strApp) ;
		wchar_t wstrArg [ 256 ] ;
		StrPCopy(wstrArg, FileName) ;
		StrCat(wstrApp, wstrArg) ;
#endif
		StrPCopy(wstrApp, strApp + FileName) ;
		if (CrearCmdWait(
				wstrApp,
				L".", L"", L"hola", true, 1024, 768, false
			)
		   )
		return ;
	}  
//	system("start Y:\\log_e9.crg.csv") ;
	AnsiString startCronograma = "start " + FileName ;
	char strCmd [ 256 ] ;
	StrPCopy((char *)&strCmd, startCronograma) ;
	system(strCmd) ;
//	system("start cmd.exe") ;	
	return ;
}


/* ------------------------------------------------------------------------*/
/* void mostrar_fichero_pdf ( FileName, pagina )                           */
/* ------------------------------------------------------------------------*/

void mostrar_fichero_pdf ( AnsiString FileName, int pagina )
{
	AnsiString strApp = "SumatraPDF.exe" ;
	strApp = strApp + Format(" -page %d ", ARRAYOFCONST((pagina))) ;
	wchar_t wstrApp [ 256 ] ;
	StrPCopy(wstrApp, strApp + FileName) ;
	CrearCmdWait(wstrApp, L".", L"", L"hola", true, 1024, 768, false) ;
}
