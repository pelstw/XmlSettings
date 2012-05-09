#include "Param.h"

#include "cinder/Vector.h"
#include "cinder/Color.h"

namespace cinder { namespace settings { 
	
	std::string Param::getAsString() 
	{ 
		std::string result = "";
		
		if ( mType == PARAM_INT )
			return ci::toString(*static_cast<int*>(mParam));
		else if ( mType == PARAM_FLOAT )
			return ci::toString(*static_cast<float*>(mParam));
		else if ( mType == PARAM_DOUBLE )
			return ci::toString(*static_cast<double*>(mParam));
		else if ( mType == PARAM_BOOL )
			return ci::toString(*static_cast<bool*>(mParam));
		else if ( mType == PARAM_VEC3F )
			return ci::toString(*static_cast<ci::Vec3f*>(mParam));
		else if ( mType == PARAM_COLOR )
			return ci::toString(*static_cast<ci::Color*>(mParam));
		else if ( mType == PARAM_COLORA )
			return ci::toString(*static_cast<ci::ColorA*>(mParam));
		else if ( mType == PARAM_STRING )
			return *static_cast<std::string*>(mParam);

		return result;
	}
	
	void Param::addXmlNode( ci::XmlTree &root )
	{
		ci::XmlTree *category;
		bool categoryExists = root.hasChild( mCategory );
		
		if ( categoryExists )
			category = &root.getChild( mCategory ); 
		else
			category = new ci::XmlTree( mCategory, "" );

		ci::XmlTree node( mName , "" );
		node.setAttribute( "type", getType() );
		
		if ( mType == PARAM_INT )
			node.setAttribute( "value", *static_cast<int*>(mParam) );
		else if ( mType == PARAM_FLOAT )
			node.setAttribute( "value", *static_cast<float*>(mParam) );
		else if ( mType == PARAM_DOUBLE )
			node.setAttribute( "value", *static_cast<double*>(mParam) );
		else if ( mType == PARAM_BOOL )
			node.setAttribute( "value", *static_cast<bool*>(mParam) );
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
		else if ( mType == PARAM_STRING )
			node.setAttribute( "value", *static_cast<std::string*>(mParam) );

		node.setAttribute( "show", mShow );
		node.setAttribute( "option", mOption );
		node.setAttribute( "category", mCategory );

		category->push_back( node );

		if ( !categoryExists )
		{
			root.push_back( *category );
			delete category;
		}
	}

} } // namespace cinder::settings