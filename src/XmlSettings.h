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

#include "cinder/Xml.h"
#include "cinder/Utilities.h"
#include "cinder/params/Params.h"

/*
 <?xml version="1.0" encoding="utf-8"?>
 <settings>
 <float name="resolution" width="1706" height="960" />
 <bool name="runDebugMode" value="1" />
 </settings>
 */

namespace nocte { 
	
	static const std::string typeString[10] = { 
		"int", 
		"float", 
		"double", 
		"bool", 
		"Vec2f", 
		"Vec3f", 
		"Color", 
		"ColorA", 
		"Font", 
		"string" 
	};


	class Param {

		friend class XmlSettings;

	public:

		enum ParamType {
			PARAM_INT,
			PARAM_FLOAT,
			PARAM_DOUBLE,
			PARAM_BOOL,
			PARAM_VEC2F,
			PARAM_VEC3F,
			PARAM_COLOR,
			PARAM_COLORA,
			PARAM_FONT,
			PARAM_STRING
		};
		
		Param( const std::string &name, void *param, ParamType paramType, bool show = false, const std::string &option = "" ) :
			mName(name), mParam(param), mType(paramType), mShow(show), mOption(option)
		{
		};

		template <typename T>
		T getValue() { return *static_cast<T*>(mParam); };

		template <typename T> 
		void setValue(T val) { *static_cast<T*>(mParam) = val; };

		std::string getName() { return mName; };

		std::string	getType() { return typeString[mType]; };
		
		std::string getAsString() { 

			if ( mType == PARAM_INT )
				return ci::toString(*static_cast<int*>(mParam));
			else if ( mType == PARAM_FLOAT )
				return ci::toString(*static_cast<float*>(mParam));
			else if ( mType == PARAM_DOUBLE )
				return ci::toString(*static_cast<double*>(mParam));
			else if ( mType == PARAM_BOOL )
				return ci::toString(*static_cast<bool*>(mParam));
// 			else if ( mType == PARAM_VEC2F )
// 				return ci::toString(*static_cast<ci::Vec2f*>(mParam));
			else if ( mType == PARAM_VEC3F )
				return ci::toString(*static_cast<ci::Vec3f*>(mParam));
			else if ( mType == PARAM_COLOR )
				return ci::toString(*static_cast<ci::Color*>(mParam));
			else if ( mType == PARAM_COLORA )
				return ci::toString(*static_cast<ci::ColorA*>(mParam));
// 			else if ( mType == PARAM_FONT )
// 				return "TODO !!!!";//ci::toString(*static_cast<ci::ColorA*>(mParam));
			else if ( mType == PARAM_STRING )
				return *static_cast<std::string*>(mParam);
		};


		ci::XmlTree getXmlNode()
		{
			ci::XmlTree node( mName, "" );
			node.setAttribute( "type", getType() );
			
			if ( mType == PARAM_INT )
				node.setAttribute( "value", *static_cast<int*>(mParam) );
			else if ( mType == PARAM_FLOAT )
				node.setAttribute( "value", *static_cast<float*>(mParam) );
			else if ( mType == PARAM_DOUBLE )
				node.setAttribute( "value", *static_cast<double*>(mParam) );
			else if ( mType == PARAM_BOOL )
				node.setAttribute( "value", *static_cast<bool*>(mParam) );
// 			else if ( mType == PARAM_VEC2F )
// 			{
// 				node.setAttribute( "x", static_cast<ci::Vec2f*>(mParam)->x );
// 				node.setAttribute( "y", static_cast<ci::Vec2f*>(mParam)->y );
// 			}
			else if ( mType == PARAM_VEC3F )
			{
				node.setAttribute( "x", static_cast<ci::Vec3f*>(mParam)->x );
				node.setAttribute( "y", static_cast<ci::Vec3f*>(mParam)->y );
				node.setAttribute( "z", static_cast<ci::Vec3f*>(mParam)->z );
			}
			else if ( mType == PARAM_COLOR )
			{
				node.setAttribute( "r", static_cast<ci::Color*>(mParam)->r );
				node.setAttribute( "g", static_cast<ci::Color*>(mParam)->g );
				node.setAttribute( "b", static_cast<ci::Color*>(mParam)->b );
			}
			else if ( mType == PARAM_COLORA )
			{
				node.setAttribute( "r", static_cast<ci::ColorA*>(mParam)->r );
				node.setAttribute( "g", static_cast<ci::ColorA*>(mParam)->g );
				node.setAttribute( "b", static_cast<ci::ColorA*>(mParam)->b );
				node.setAttribute( "a", static_cast<ci::ColorA*>(mParam)->a );
			}
// 			else if ( mType == PARAM_FONT )
// 			{
// 				node.setAttribute( "typeface", static_cast<ci::Font*>(mParam)->getName() );
// 				node.setAttribute( "size", static_cast<ci::Font*>(mParam)->getSize() );
// 			}
			else if ( mType == PARAM_STRING )
				node.setAttribute( "value", *static_cast<std::string*>(mParam) );

			node.setAttribute( "show", mShow );
			node.setAttribute( "option", mOption );

			return node;

		};
		
		
	protected:
		
		std::string		mName;
		void			*mParam;
		ParamType		mType;
		bool			mShow;
		std::string		mOption;

	};


	class XmlSettings {
		
	public:
		
		XmlSettings() 
		{ 
			mFilename = "";
			mParamGUI = ci::params::InterfaceGl("Input", ci::Vec2i(280, 560));
			mParamGUI.setOptions();
			mParamGUI.addSeparator( "Tada" );
			mParamGUI.addText( "Tadaa");
		};


		void parseNode( ci::XmlTree node )
		{

			std::string type	= node.getAttributeValue<std::string>("type");
			std::string name	= node.getTag();
			bool paramFound		= false;

			for( int k=0; k < mParams.size(); k++)
			{
				if ( mParams[k]->getName() == name )
				{
					paramFound = true;

					if ( type == "int" )
						mParams[k]->setValue<int>( node.getAttributeValue<int>("value") );

					else if ( type == "float" )
						mParams[k]->setValue<float>( node.getAttributeValue<float>("value") );

					else if ( type == "double" )
						mParams[k]->setValue<double>( node.getAttributeValue<double>("value") );

					//else if ( type == "Vec2f" )
					//	mParams[k]->setValue<ci::Vec2f>( ci::Vec2f( node.getAttributeValue<float>("x"), node.getAttributeValue<float>("y") ) );

					else if ( type == "Vec3f" )
						mParams[k]->setValue<ci::Vec3f>( ci::Vec3f( node.getAttributeValue<float>("x"), node.getAttributeValue<float>("y"), node.getAttributeValue<float>("z") ) );

					else if ( type == "bool" )
						mParams[k]->setValue<bool>( node.getAttributeValue<bool>("value") );

					else if ( type == "Color" )
						mParams[k]->setValue<ci::Color>( ci::Color( node.getAttributeValue<float>("r"), node.getAttributeValue<float>("g"), node.getAttributeValue<float>("b") ) );

					else if ( type == "ColorA" )
						mParams[k]->setValue<ci::ColorA>( ci::ColorA( node.getAttributeValue<float>("r"), node.getAttributeValue<float>("g"), node.getAttributeValue<float>("b"), node.getAttributeValue<float>("a") ) );

					//else if ( type == "Font" )
					//	mParams[k]->setValue<ci::Font>( ci::Font( node.getAttributeValue<std::string>("typeface"), node.getAttributeValue<int>("size") ) );

					else if ( type == "string" )
						mParams[k]->setValue<std::string>( node.getAttributeValue<std::string>("value") );
				} 
			}
			
			if ( !paramFound )
			{
				if ( type == "int" )
					addParam( name, new int(node.getAttributeValue<int>("value")), 
					node.getAttributeValue<bool>("show"), 
					node.getAttributeValue<std::string>("option"));

				else if ( type == "float" )
					addParam( name, new float(node.getAttributeValue<float>("value")),
					node.getAttributeValue<bool>("show"), 
					node.getAttributeValue<std::string>("option"));

				else if ( type == "double" )
					addParam( name, new double(node.getAttributeValue<double>("value")),
					node.getAttributeValue<bool>("show"), 
					node.getAttributeValue<std::string>("option"));

				//else if ( type == "Vec2f" )
				//	addParam( name, new ci::Vec2f( node.getAttributeValue<float>("x"), node.getAttributeValue<float>("y") ) );

				else if ( type == "Vec3f" )
					addParam( name, new ci::Vec3f( node.getAttributeValue<float>("x"), 
					node.getAttributeValue<float>("y"), 
					node.getAttributeValue<float>("z") ), 
					node.getAttributeValue<bool>("show"), 
					node.getAttributeValue<std::string>("option"));

				else if ( type == "bool" )
					addParam( name, new bool(node.getAttributeValue<bool>("value")),
					node.getAttributeValue<bool>("show"), 
					node.getAttributeValue<std::string>("option"));

				else if ( type == "Color" )
					addParam( name, new ci::Color( node.getAttributeValue<float>("r"), 
					node.getAttributeValue<float>("g"), 
					node.getAttributeValue<float>("b") ), 
					node.getAttributeValue<bool>("show"), 
					node.getAttributeValue<std::string>("option"));

				else if ( type == "ColorA" )
					addParam( name, new ci::ColorA( node.getAttributeValue<float>("r"), 
					node.getAttributeValue<float>("g"), 
					node.getAttributeValue<float>("b"), 
					node.getAttributeValue<float>("a") ), 
					node.getAttributeValue<bool>("show"),
					node.getAttributeValue<std::string>("option"));

				//else if ( type == "Font" )
				//	addParam( name, new ci::Font( node.getAttributeValue<std::string>("typeface"), node.getAttributeValue<int>("size") ) );

				else if ( type == "string" )
					addParam( name, new std::string(node.getAttributeValue<std::string>("value")), 
					node.getAttributeValue<bool>("show"),
					node.getAttributeValue<std::string>("option"));
			}
		};


		bool hasParam(std::string name) 
		{
			for( int k=0; k < mParams.size(); k++)
				if ( mParams[k]->getName() == name )
					return true;
			
			return false;
		};


		Param* getParam(std::string name) 
		{
			for( int k=0; k < mParams.size(); k++)
				if ( mParams[k]->getName() == name )
					return mParams[k];
			
			return NULL;
		};


		template <class T>
		T		getValueByName( const std::string &name ) 
		{ 
			Param *p = getParam(name);
			
			if ( !p )
			{
				ci::app::console() << "param not found: " << name << std::endl;
				exit(-1); // throw exception instead
			}
			
			return *static_cast<T*>( p->mParam ); 
		};


		void drawDebug()
		{
			ci::TextLayout textLayout = ci::TextLayout();
			textLayout.setColor( ci::Color::white() );
			textLayout.clear( ci::Color::black() );
			textLayout.setBorder(10, 10);
			textLayout.setLeadingOffset(2);
			textLayout.setFont( ci::Font("Verdana", 13) );
			
			textLayout.addLine( "XML SETTINGS" );
			textLayout.addLine( " " );
			
			for( int k=0; k < mParams.size(); k++)
				textLayout.addLine( mParams[k]->getType() + " \t" + mParams[k]->getName() + " \t" + mParams[k]->getAsString() );
			
			ci::gl::Texture tex = ci::gl::Texture(textLayout.render(true));
			ci::gl::draw(tex);
		};
		

		void drawGUI()
		{
			mParamGUI.draw();
		};


		void load( std::string filename = "" ) 
		{
			
			if ( filename != "" )
				mFilename = filename;
			
			if ( mFilename == "" )
			{
				ci::app::console() << "cannot load XML settings, specify filename" << std::endl;
				return;
			}
			
			try {
				mXmlAsset = ci::XmlTree( ci::loadFile(mFilename) );
			}
			catch ( ... ) {
				ci::app::console() << "failed to load XML settings file: " << mFilename << std::endl;
				return;
			}
			
			std::list<ci::XmlTree> settings = mXmlAsset.getChild("/settings").getChildren();
			
			for(std::list<ci::XmlTree>::iterator nodeIt = settings.begin(); nodeIt != settings.end(); ++nodeIt)
				parseNode( *nodeIt );

			ci::app::console() << "XML settings loaded from: " << mFilename << std::endl;
		};
		
		
		void save( std::string filename = "" )
		{	
			if ( filename == "" )
				filename = ci::app::getAppPath().parent_path().generic_string() + "/appSettings.xml";
			
			mFilename = filename;
			
			mXmlAsset = ci::XmlTree("settings", "");
			
			for(int k=0; k < mParams.size(); k++)
				mXmlAsset.push_back( mParams[k]->getXmlNode() );
			
			mXmlAsset.write( ci::writeFile( mFilename ) );
			
			ci::app::console() << "XML settings saved: " << mFilename << std::endl;
			
		};


		void addParam( const std::string &name, int *param, bool show, const std::string &option = "" )
		{ addOrBind(name, param, Param::PARAM_INT, show, option); };

		void addParam( const std::string &name, float *param, bool show, const std::string &option = "" )
		{ addOrBind(name, param, Param::PARAM_FLOAT, show, option); };

		void addParam( const std::string &name, double *param, bool show, const std::string &option = "" )
		{ addOrBind(name, param, Param::PARAM_DOUBLE, show, option); };
		
		void addParam( const std::string &name, bool *param, bool show, const std::string &option = "" )
		{ addOrBind(name, param, Param::PARAM_BOOL, show, option); };

		//void	addParam( const std::string &name, ci::Vec2f *param, bool show )	{ addOrBind(name, param, Param::PARAM_VEC2F, show); };
		
		void addParam( const std::string &name, ci::Vec3f *param, bool show, const std::string &option = "" )
		{ addOrBind(name, param, Param::PARAM_VEC3F, show, option); };

		void addParam( const std::string &name, ci::Color *param, bool show, const std::string &option = "" )
		{ addOrBind(name, param, Param::PARAM_COLOR, show, option); };

		void addParam( const std::string &name, ci::ColorA *param, bool show, const std::string &option = "" )
		{ addOrBind(name, param, Param::PARAM_COLORA, show, option); };

		//void	addParam( const std::string &name, ci::Font *param, bool show )		{ addOrBind(name, param, Param::PARAM_FONT, show); };
		
		void addParam( const std::string &name, std::string *param, bool show, const std::string &option = "" )
		{ addOrBind(name, param, Param::PARAM_STRING, show, option); };


	private:
		
		template<typename T>
		void addOrBind( const std::string &name, T* param, Param::ParamType paramType, bool show, const std::string &option )
		{
			Param *p = getParam(name);

			if ( p )
			{
				p->mParam = param;
				p->mShow = show;
				p->mOption = option;
				mParamGUI.delParam( name );
			}
			else
			{
				mParams.push_back( new Param( name, param, paramType, show, option ) );
			}

			if ( show )
			{
				mParamGUI.addParam( name, param, option );
			}
		}
		
		std::string					mFilename;
		std::vector<Param*>			mParams;
		ci::XmlTree					mXmlAsset;
		std::vector<ci::XmlTree>	mXmlNodes;
		ci::params::InterfaceGl		mParamGUI;
	};

}