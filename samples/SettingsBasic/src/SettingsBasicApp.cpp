/*
 *  SettingsBasicApp.cpp
 *  XmlSettings
 *
 *  Created by Andrea Cuius on 26/11/2011.
 *  Nocte Studio Copyright 2011 . All rights reserved.
 *	www.nocte.co.uk
 *
 */

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include "Resources.h"
#include "XmlSettings.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class SettingsBasicApp : public AppBasic {
  public:
	void setup();
	void keyDown( KeyEvent event );
	void draw();
	
	ci::settings::XmlSettings	*mSettings;
	
	int			mTestInt32;
	float		mTestFloat;
	double		mTestDouble;
	bool		mTestBool;
	
	Vec3f		mTestVec3f;
	
	Color		mTestColor;
	ColorA		mTestColorA;
	
	string		mTestString;
};

void SettingsBasicApp::setup()
{
	mSettings = new ci::settings::XmlSettings();							// create a new instance, at this point XmlSettings doesn't have any file associated
	
	mSettings->addParam( "mTestFloat",	&mTestFloat, "test", true, "min=0.0 max=5.0 step=0.5" );				// add a new variable to the XmlSettings
	mSettings->addParam( "mTestDouble",	&mTestDouble, "test", false );
	mSettings->addParam( "mTestBool",	&mTestBool, "", true );
	mSettings->addParam( "mTestVec3f",	&mTestVec3f, "input", true );
	mSettings->addParam( "mTestColor",	&mTestColor, "input", true );
	mSettings->addParam( "mTestColorA",	&mTestColorA, "output", true );
	mSettings->addParam( "mTestString",	&mTestString, "output", true );
	
	mTestInt32	= 0;
	mTestFloat	= 0.0f;
	mTestDouble	= 0.123456789;
	mTestBool	= false;
	mTestVec3f	= Vec3f::zero();
	mTestColor	= Color::black();
	mTestColorA	= ColorA::black();
	mTestString	= "zero";
	
	string filename = getOpenFilePath().generic_string();
	
	if ( filename != "" )
		mSettings->load(filename);									// load the xml settings file, this method override all the current values in XmlSettings
	
	mSettings->addParam( "mTestInt32",	&mTestInt32, "", true );				// if mTestInt32 has been loaded already from the xml file ( load() ), but not added yet, addParam() just replace the pointer
	console() << mTestInt32 << endl;
	
	mTestInt32	= 666;
	console() << mTestInt32 << endl;

	console() << mSettings->getValueByName<float>("mTestFloat") << endl;
	console() << mSettings->getValueByName<ColorA>("mTestColorA") << endl;
}


void SettingsBasicApp::keyDown( KeyEvent event )
{
	char c = event.getChar();
	
	if ( c == 'l' )													// Load a new xml settings file
	{
		string filename = getOpenFilePath().generic_string();
		if ( filename != "" )
			mSettings->load(filename);
	}
	
	else if ( c == 'r' )											// Reload the current xml settings file, if none has been loaded, it doesn't do anything
		mSettings->load();
	
	else if ( c == 's' )											// Save the current settings to the app root folder, the default filename is appSettings.xml, you can also specify another one
		mSettings->save();
		
}


void SettingsBasicApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	
	if ( mSettings )
	{
		mSettings->drawDebug();
		mSettings->drawGUI();
	}

}


CINDER_APP_BASIC( SettingsBasicApp, RendererGl )
