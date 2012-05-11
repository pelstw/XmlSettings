/*
 *  XmlSettings.h
 *  XmlSettings
 *
 *  Created by Andrea Cuius on 26/11/2011.
 *  Nocte Studio Copyright 2011 . All rights reserved.
 *	www.nocte.co.uk
 *
 */

#pragma once

#include "Param.h"

#include "cinder/Xml.h"
#include "cinder/params/Params.h"
#include "cinder/Utilities.h"

/*
 <?xml version="1.0" encoding="utf-8"?>
 <settings>
 <float name="resolution" width="1706" height="960" />
 <bool name="runDebugMode" value="1" />
 </settings>
 */

namespace cinder { namespace settings { 
	
	class XmlSettings {

	public:

		XmlSettings() 
		{ 
			mFilename = "";
			mParamGUI = ci::params::InterfaceGl( "App parameters", Vec2i( 200, 400 ) );
			mParamGUI.setOptions();
		};

		~XmlSettings()
		{
			for( std::vector<Param*>::iterator iter = mParams.begin(); iter != mParams.end(); ++iter )
				delete * iter;
		}

		void parseNode( ci::XmlTree node );

		bool hasParam(std::string name);

		Param* getParam(std::string name);

		template <typename T>
		T	getValueByName( const std::string &name )
		{ 
			Param *p = getParam(name);

			if ( !p )
			{
				ci::app::console() << "param not found: " << name << std::endl;
				exit(-1); // throw exception instead
			}

			return *static_cast<T*>( p->mParam ); 
		};

		void drawDebug();
		void drawGUI();

		void load( std::string filename = "" );

		void save( std::string filename = "" );

		void addParam( const std::string &name, int *param, const std::string &category, bool show = false, const std::string &option = "" );
		void addParam( const std::string &name, float *param, const std::string &category, bool show = false, const std::string &option = "" );
		void addParam( const std::string &name, double *param, const std::string &category, bool show = false, const std::string &option = "" );
		void addParam( const std::string &name, bool *param, const std::string &category, bool show = false, const std::string &option = "" );
		void addParam( const std::string &name, ci::Vec3f *param, const std::string &category, bool show = false, const std::string &option = "" );
		void addParam( const std::string &name, ci::Vec2f *param, const std::string &category, bool show = false, const std::string &option = "" );
		void addParam( const std::string &name, ci::Color *param, const std::string &category, bool show = false, const std::string &option = "" );
		void addParam( const std::string &name, ci::ColorA *param, const std::string &category, bool show = false, const std::string &option = "" );
		void addParam( const std::string &name, std::string *param, const std::string &category, bool show = false, const std::string &option = "" );

	private:
		
		template<typename T>
		void addOrBind( const std::string &name, T *param, Param::ParamType paramType, const std::string &category, bool show, const std::string &option )
		{
			std::string catg = category;

			if ( catg == "" ) 
			{
				catg = "_";
			}

			Param *p = getParam(name);

			if ( p )
			{
				delete p->mParam;
				p->mParam = param;
				p->mShow = show;
				p->mOption = option;
				p->mCategory = catg;
			}
			else
			{
				mParams.push_back( new Param( name, param, paramType, catg, show, option ) );
			}

			mParamsChanged = true;
		}
		
		void buildXMLTree();
		void buildGUI();
		
		std::string					mFilename;
		std::vector<Param*>			mParams;
		ci::XmlTree					mXmlAsset;
		ci::params::InterfaceGl		mParamGUI;
		bool						mParamsChanged;
	};

} } // namespace cinder::settings