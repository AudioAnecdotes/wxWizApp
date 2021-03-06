// WizAppData.cpp: implementation of the WizAppData class.
//
//////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif

#include "wx/tokenzr.h"
#include "wx/file.h"
#include "wx/image.h"

#include "WIZAPP.xpm"
#include "wxWizApp.xpm"

#include "defines.h"
#include "WizAppData.h"

#include <stdlib.h> // getenv()
bool myWXGetEnv(const wxString& var, wxString *value)
{
	// for wxWidgets built on cygwin, use the UNIX getevnv
	// because cygwin seems to have stopped exporting windows
	// compatible environments to cygwin apps (like wxWizApp built
	// on cygwin)
	// This should work fine on unix systems too

	wxChar *p = getenv(var);
	if(!p)
		return false;

	if(value)
		*value = p;

	// return(0 != p);
	return(true); // XXX shouldn't this be conditional?
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WizAppData::WizAppData()
{
	ico="";
	bmp="";
	errlevel=256;
	sig.Printf("%s %s",APP_TITLE,APP_VERSION);
	title=APP_TITLE;
	// wxInitAllImageHandlers(); // png fails on cygwin
	// so instead only init what we need:
	wxImage::AddHandler(new wxBMPHandler);
	wxImage::AddHandler(new wxXPMHandler);

	bitmap=wxBitmap(wizapp_xpm);
	icon=wxIcon(wxWizApp_xpm);
	BuildInputEnvironment();
	GetLabels();
	GetFile();
}

WizAppData::~WizAppData()
{

}

void WizAppData::BuildInputEnvironment()
{
	wxString prefix;
	wxString tmp;

    if (myWXGetEnv(VAR_PREFIX, &prefix) == 0)
        prefix = DEF_PREFIX;
	
	wxString var;
	var.Printf("%s%s",prefix.c_str(),VAR_TEXT);
	myWXGetEnv(var,&text);
	var.Printf("%s%s",prefix.c_str(),VAR_TITLE);
	myWXGetEnv(var,&title);
	var.Printf("%s%s",prefix.c_str(),VAR_BMP);
	myWXGetEnv(var,&bmp);
	var.Printf("%s%s",prefix.c_str(),VAR_ICO);
	myWXGetEnv(var,&ico);
	var.Printf("%s%s",prefix.c_str(),VAR_SIG);
	myWXGetEnv(var,&sig);
	var.Printf("%s%s",prefix.c_str(),VAR_SOUND);
	myWXGetEnv(var,&sound);
	var.Printf("%s%s",prefix.c_str(),VAR_FILE);
	myWXGetEnv(var,&file);
	var.Printf("%s%s",prefix.c_str(),VAR_INPUT);
	myWXGetEnv(var,&input);
	var.Printf("%s%s",prefix.c_str(),VAR_LABELS);
	myWXGetEnv(var,&labels);
	var.Printf("%s%s",prefix.c_str(),VAR_OUTPUT);
	myWXGetEnv(var,&output);
	var.Printf("%s%s",prefix.c_str(),VAR_OUTNUM);
	myWXGetEnv(var,&outnum);
	var.Printf("%s%s",prefix.c_str(),VAR_BAT);
	myWXGetEnv(var,&bat);
	var.Printf("%s%s",prefix.c_str(),VAR_LISTSEP);
	myWXGetEnv(var,&tmp);
	if (tmp.c_str()[0]=='\0')
		listsep=DEF_LISTSEP;
	else
		listsep=tmp.c_str()[0];
	var.Printf("%s%s",prefix.c_str(),VAR_EOL);
	tmp="";
	myWXGetEnv(var,&tmp);
	if (tmp.c_str()[0]=='\0')
		eol=DEF_EOL;
	else
		eol=tmp.c_str()[0];

	wxString eols=eol;
	input.Replace(eols,"\n");
	text.Replace(eols,"\n");
}

void WizAppData::GetLabels()
{
	wxArrayString labs;
	int num=ExplodeList(labels,listsep,labs);

    if (num > 0 && *labs[0])
        labelback = labs[0];
    else
        labelback = LABEL_BACK;
    if (num > 1 && *labs[1])
        labelnext = labs[1];
    else
        labelnext = LABEL_NEXT;
    if (num > 2 && *labs[2])
        labelfinish = labs[2];
    else
        labelfinish = LABEL_FINISH;
    if (num > 3 && *labs[3])
        labelcancel = labs[3];
    else
        labelcancel = LABEL_CANCEL;
    if (num > 4  && *labs[4])
        labelbrowse = labs[4];
    else
        labelbrowse = LABEL_BROWSE;
	labs.Clear();
}

void WizAppData::GetFile()
{
	if (file.Len()>0)
	{
		wxFile fd;
		if (fd.Open(file))
		{
			char* buf=new char[fd.Length()+1];
			buf[fd.Length()]=0;
			fd.Read(buf,fd.Length());
			fd.Close();
			fileinput=buf;
			delete buf;
		}
	}
}

int WizAppData::ExplodeList(const wxString str,char sep,wxArrayString& items)
{
	items.Clear();
	wxStringTokenizer toks(str,wxString(sep));
	int rv=toks.CountTokens();
	while (toks.HasMoreTokens())
	{
		wxString str=toks.GetNextToken();
		items.Add(str);
	}
	return rv;
}
