/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

#include <conprint.h>
#include <MAUtil/Moblet.h>
#include <mavsprintf.h>

#include "Soap.h"

using namespace MAUtil;

// This request will return a the result of a multiplication.

static const char sUrl[] = "http://brzeszczot.net/soap_server";//"http://modev.mine.nu:12346/";
static const char sData[] =
/*
"<soapenv:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:urn=\"urn:wsdl\">"
	"<soapenv:Header/>"
		"<soapenv:Body>"
		      "<urn:up soapenv:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
		         "<str xsi:type=\"xsd:string\">Ala ma kota</str>"
		      "</urn:up>"
		   "</soapenv:Body>"
		"</soapenv:Envelope>"
;
*/

		"<ns:Get>\n"
		"<data SOAP-ENC:arrayType=\"xsd:string[3]\" xsi:type=\"SOAP-ENC:Array\">\n"
		"<item xsi:type=\"xsd:string\">Ala </item>"
		"<item xsi:type=\"xsd:string\">ma </item>"
		"<item xsi:type=\"xsd:string\">kota </item>"
		"</data>\n"
		"</ns:Get>\n"
		;

/*
		"<ns:Get>\n"
		"<data SOAP-ENC:arrayType=\"ns2:Map[1]\" xsi:type=\"SOAP-ENC:Array\">\n"
		"<item xsi:type=\"ns2:Map\">"
		"<item>"
	        "<key xsi:type=\"xsd:string\">one</key>"
	        "<value xsi:type=\"xsd:string\">ala </value>"
		"</item>"
	    "<item>"
	        "<key xsi:type=\"xsd:string\">two</key>"
	        "<value xsi:type=\"xsd:string\">ma </value>"
		"</item>"
	    "<item>"
	        "<key xsi:type=\"xsd:string\">three</key>"
	        "<value xsi:type=\"xsd:string\">kota</value>"
		"</item>"
		"</item>"
		"</data>\n"
		"</ns:Get>\n"
		;
*/
/*
		"<ns:Get>\n"
		//"<data xsi:type=\"xsd:anyType\">ala </data>"
		"<str xsi:type=\"xsd:string\">Ala ma kota</str>"
		"</ns:Get>\n"
		;
*/
/*
"<ns:mul>\n"
	"<param-7>6.0</param-7>\n"
	"<param-8>7.0</param-8>\n"
"</ns:mul>\n"
;
*/

class MyMoblet : public Moblet, SoapListener {
private:
	SoapRequest mSoap;
public:
	MyMoblet() {
		mSoap.start(sUrl, sData, sizeof(sData) - 1, this);
	}

	void soapError(int code) {
		// an error code -6 (CONNERR_CLOSED) is normal after soapEnd(),
		// as the server closes the HTTP connection.
		if(code == CONNERR_CLOSED) {
			printf("Connection closed by server.\n");
			return;
		}
		printf("soapError(%i)\n", code);
	}
	void soapEnd() {
		printf("soapEnd()\n");
	}

	void mtxEncoding(const char* name) {
		printf("encoding '%s'\n", name);
	}
	void mtxTagStart(const char* name, int len) {
		printf("start '%s'\n", name);
	}
	void mtxTagAttr(const char* attrName, const char* attrValue) {
		printf("attr %s='%s'\n", attrName, attrValue);
	}
	void mtxTagStartEnd() {
		//printf("startEnd\n");
	}
	void mtxTagData(const char* data, int len) {
		printf("data '%s'\n", data);
	}
	void mtxTagEnd(const char* name, int len) {
		printf("end '%s'\n", name);
	}
	void mtxParseError(int offset) {
		printf("parseError %i\n", offset);
	}
	void mtxEmptyTagEnd() {
		printf("emptyTagEnd\n");
	}
	unsigned char mtxUnicodeCharacter(int unicode) {
		char c = XmlListener::mtxUnicodeCharacter(unicode);
		if(c == 0) {	//we don't know how to convert this character
			c = '?';
			lprintfln("Unknown unicode character: 0x%x", unicode);
		}
		return c;
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		switch(keyCode) {
		case MAK_SOFTRIGHT:
		case MAK_0:
		case MAK_BACK:
			maExit(0);
		}
	}

	void keyReleaseEvent(int keyCode, int nativeCode) {
	}
};

extern "C" int MAMain() {
	InitConsole();
	Moblet::run(new MyMoblet());
	return 0;
}
