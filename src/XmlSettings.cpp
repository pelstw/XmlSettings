#include "XmlSettings.h"

#include "cinder/Text.h"
#include "cinder/gl/Texture.h"
#include "cinder/app/AppBasic.h"

namespace cinder { namespace settings { 
	
	void XmlSettings::parseNode( ci::XmlTree node )
	{
		std::string type		= node.getAttributeValue<std::string>("type");
		std::string name		= node.getTag();
		std::string category	= node.getAttributeValue<std::string>("category");
		bool show				= node.getAttributeValue<bool>("show"); 
		std::string	option		= node.getAttributeValue<std::string>("option");
		bool paramFound		= false;

		for( unsigned int k=0; k < mParams.size(); k++)
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

				else if ( type == "Vec3f" )
					mParams[k]->setValue<ci::Vec3f>( ci::Vec3f( node.getAttributeValue<float>("x"), node.getAttributeValue<float>("y"), node.getAttributeValue<float>("z") ) );

				else if ( type == "bool" )
					mParams[k]->setValue<bool>( node.getAttributeValue<bool>("value") );

				else if ( type == "Color" )
					mParams[k]->setValue<ci::Color>( ci::Color( node.getAttributeValue<float>("r"), node.getAttributeValue<float>("g"), node.getAttributeValue<float>("b") ) );

				else if ( type == "ColorA" )
					mParams[k]->setValue<ci::ColorA>( ci::ColorA( node.getAttributeValue<float>("r"), node.getAttributeValue<float>("g"), node.getAttributeValue<float>("b"), node.getAttributeValue<float>("a") ) );

				else if ( type == "string" )
					mParams[k]->setValue<std::string>( node.getAttributeValue<std::string>("value") );
			} 
		}
		
		if ( !paramFound )
		{
			if ( type == "int" )
				addParam( name, new int( node.getAttributeValue<int>("value") ), category, show, option );

			else if ( type == "float" )
				addParam( name, new float( node.getAttributeValue<float>("value") ), category, show, option );

			else if ( type == "double" )
				addParam( name, new double( node.getAttributeValue<double>("value") ), category, show, option );

			else if ( type == "Vec3f" )
				addParam( name, new ci::Vec3f( node.getAttributeValue<float>("x"), node.getAttributeValue<float>("y"), node.getAttributeValue<float>("z") ), category, show, option );

			else if ( type == "bool" )
				addParam( name, new bool( node.getAttributeValue<bool>("value") ), category, show, option );

			else if ( type == "Color" )
				addParam( name, new ci::Color( node.getAttributeValue<float>("r"), node.getAttributeValue<float>("g"), node.getAttributeValue<float>("b") ), category, show, option );

			else if ( type == "ColorA" )
				addParam( name, new ci::ColorA( node.getAttributeValue<float>("r"), node.getAttributeValue<float>("g"), node.getAttributeValue<float>("b"), node.getAttributeValue<float>("a") ), category, show, option );

			else if ( type == "string" )
				addParam( name, new std::string(node.getAttributeValue<std::string>("value")), category, show, option );
		}
	}

	bool XmlSettings::hasParam(std::string name) 
	{
		for( unsigned int k=0; k < mParams.size(); k++)
			if ( mParams[k]->getName() == name )
				return true;
		
		return false;
	}

	Param* XmlSettings::getParam(std::string name) 
	{
		for( unsigned int k=0; k < mParams.size(); k++)
			if ( mParams[k]->getName() == name )
				return mParams[k];
		
		return NULL;
	}

	void XmlSettings::drawDebug()
	{
		ci::TextLayout textLayout = ci::TextLayout();
		textLayout.setColor( ci::Color::white() );
		textLayout.clear( ci::Color::black() );
		textLayout.setBorder(10, 10);
		textLayout.setLeadingOffset(2);
		textLayout.setFont( ci::Font("Verdana", 13) );
		
		textLayout.addLine( "XML SETTINGS" );
		textLayout.addLine( " " );
		
		for( unsigned int k=0; k < mParams.size(); k++)
			textLayout.addLine( mParams[k]->getType() + " \t" + mParams[k]->getName() + " \t" + mParams[k]->getAsString() );
		
		ci::gl::Texture tex = ci::gl::Texture(textLayout.render(true));
		ci::gl::draw(tex);
	}

	void XmlSettings::drawGUI()
	{
		if ( mParamsChanged )
		{
			buildXMLTree();
			buildGUI();
			mParamsChanged = false;
		}

		mParamGUI.draw();
	}

	void XmlSettings::load( std::string filename ) 
	{
		if ( filename != "" )
			mFilename = filename;
		
		if ( mFilename == "" )
		{
			ci::app::console() << "cannot load XML settings, specify filename" << std::endl;
			return;
		}
		
		try {
			mXmlAsset = ci::XmlTree( ci::loadFile(mFilename) ).getChild("/settings");
		}
		catch ( ... ) {
			ci::app::console() << "failed to load XML settings file: " << mFilename << std::endl;
			return;
		}
		
		for ( ci::XmlTree::Iter categoryIter = mXmlAsset.begin(); categoryIter != mXmlAsset.end(); ++categoryIter )
		{
			ci::XmlTree category = *categoryIter;

			for ( ci::XmlTree::Iter nodeIter = category.begin(); nodeIter != category.end(); ++nodeIter )
			{
				parseNode( *nodeIter );
			}
		}

		ci::app::console() << "XML settings loaded from: " << mFilename << std::endl;
	}

	void XmlSettings::save( std::string filename )
	{	
		if ( filename == "" )
			filename = ci::app::getAppPath().parent_path().generic_string() + "/appSettings.xml";
		
		mFilename = filename;
		
		buildXMLTree();
		
		mXmlAsset.write( ci::writeFile( mFilename ) );
		
		ci::app::console() << "XML settings saved: " << mFilename << std::endl;
		
	}

	void XmlSettings::buildXMLTree()
	{
		mXmlAsset = ci::XmlTree("settings", "");

		for(unsigned int k=0; k < mParams.size(); k++)
			mParams[k]->addXmlNode( mXmlAsset );
	}

	void XmlSettings::buildGUI()
	{
		for ( ci::XmlTree::Iter categoryIter = mXmlAsset.begin(); categoryIter != mXmlAsset.end(); ++categoryIter )
		{
			ci::XmlTree category = *categoryIter;
			
			mParamGUI.addText( category.getTag() );

			for ( ci::XmlTree::Iter nodeIter = category.begin(); nodeIter != category.end(); ++nodeIter )
			{
				std::string param = (*nodeIter).getTag();
				Param *p = getParam( param );
				std::string type = p->getType();
				std::string &name = p->mName;
				std::string &option = p->mOption;
				bool show = p->mShow;

				if ( type == "int" )
					mParamGUI.addParam( name, static_cast<int*>(p->mParam), option );

				else if ( type == "float" )
					mParamGUI.addParam( name, static_cast<float*>(p->mParam), option );

				else if ( type == "double" )
					mParamGUI.addParam( name, static_cast<double*>(p->mParam), option );

				else if ( type == "Vec3f" )
					mParamGUI.addParam( name, static_cast<ci::Vec3f*>(p->mParam), option );

				else if ( type == "bool" )
					mParamGUI.addParam( name, static_cast<bool*>(p->mParam), option );

				else if ( type == "Color" )
					mParamGUI.addParam( name, static_cast<ci::Color*>(p->mParam), option );

				else if ( type == "ColorA" )
					mParamGUI.addParam( name, static_cast<ci::ColorA*>(p->mParam), option );

				else if ( type == "string" )
					mParamGUI.addParam( name, static_cast<std::string*>(p->mParam), option );
			}
			mParamGUI.addSeparator();
		}
	}

	void XmlSettings::addParam( const std::string &name, int *param, const std::string &category, bool show, const std::string &option )
	{ 
		addOrBind(name, param, Param::PARAM_INT, category, show, option); 
	}

	void XmlSettings::addParam( const std::string &name, float *param, const std::string &category, bool show, const std::string &option )
	{ 
		addOrBind(name, param, Param::PARAM_FLOAT, category, show, option); 
	}

	void XmlSettings::addParam( const std::string &name, double *param, const std::string &category, bool show, const std::string &option )
	{ 
		addOrBind(name, param, Param::PARAM_DOUBLE, category, show, option);
	}

	void XmlSettings::addParam( const std::string &name, bool *param, const std::string &category, bool show, const std::string &option )
	{ 
		addOrBind(name, param, Param::PARAM_BOOL, category, show, option);
	}

	void XmlSettings::addParam( const std::string &name, ci::Vec3f *param, const std::string &category, bool show, const std::string &option )
	{
		addOrBind(name, param, Param::PARAM_VEC3F, category, show, option);
	}

	void XmlSettings::addParam( const std::string &name, ci::Color *param, const std::string &category, bool show, const std::string &option )
	{ 
		addOrBind(name, param, Param::PARAM_COLOR, category, show, option);
	}

	void XmlSettings::addParam( const std::string &name, ci::ColorA *param, const std::string &category, bool show, const std::string &option )
	{ 
		addOrBind(name, param, Param::PARAM_COLORA, category, show, option); 
	}

	void XmlSettings::addParam( const std::string &name, std::string *param, const std::string &category, bool show, const std::string &option )
	{ 
		addOrBind(name, param, Param::PARAM_STRING, category, show, option);
	}

} } // namespace cinder::settings