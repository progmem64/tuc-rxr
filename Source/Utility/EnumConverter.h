#pragma once
#include <map>
#include <string>

// just function declarations: myEnumToString + myEnumFromString
#define ENUM_CONVERT_FUNC(myEnum) \
std::string myEnum##ToString( myEnum s); \
myEnum myEnum##FromString(const std::string& s);

/* 
	function definitions of myEnumToString + myEnumFromString
	-- put this in a cpp because it creates a helper map called s_myEnum
		with {myEnum, string} pairs

	notFound: Element that should be returned if no string matches e.g. myEnum::Size
	...: Pairs: {Color::Red,"red"},{Color::Blue,"blue"}
*/
#define ENUM_CONVERT(myEnum,notFound,...)	\
static std::map< myEnum ,std::string> s_##myEnum = {	\
		__VA_ARGS__	};						\
std::string myEnum##ToString( myEnum s){	\
	auto it = s_##myEnum.find(s);			\
	if(it != s_##myEnum.end()) return it->second;	\
	return "";}								\
myEnum myEnum##FromString(const std::string& s){	\
	for(const auto& e : s_##myEnum){		\
	if(e.second == s) return e.first;}		\
	return notFound;}