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
// #include "cinder/Utilities.h"
// #include "cinder/params/Params.h"

/*
 <?xml version="1.0" encoding="utf-8"?>
 <settings>
 <float name="resolution" width="1706" height="960" />
 <bool name="runDebugMode" value="1" />
 </settings>
 */

namespace cinder { namespace settings { 
	
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
		
		Param( const std::string &name, void *param, ParamType paramType, const std::string &category = "default", bool show = false, const std::string &option = "" ) :
				mName(name), 
				mParam(param), 
				mType(paramType), 
				mShow(show), 
				mOption(option),
				mCategory(category)
		{
		};

		template <typename T>
		T getValue() { return *static_cast<T*>(mParam); };

		template <typename T> 
		void setValue(T val) { *static_cast<T*>(mParam) = val; };

		std::string getName() { return mName; };

		std::string getType() { return typeString[mType]; };

		std::string getCategory() { return mCategory; };
		
		std::string getAsString();

		void addXmlNode( ci::XmlTree &root );

	protected:

		std::string		mName;
		void			*mParam;
		ParamType		mType;
		bool			mShow;
		std::string		mOption;
		std::string		mCategory;
	};

} } // namespace cinder::settings