//
//  docspec.cpp
//  cplusplus
//
//  Created by Jack on 5/10/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#include "docspec.h"

int getType(std::string typen) {
    int type = DOCTYPE_UNKNOWN;
    if (typen.find("function") != std::string::npos) type |= DOCTYPE_FUNCTION;
    if (typen.find("class") != std::string::npos) type |= DOCTYPE_CLASS;
    if (typen.find("macro") != std::string::npos) type |= DOCTYPE_MACRO | DOCTYPE_FUNCTION;
    if (typen.find("enum") != std::string::npos) type |= DOCTYPE_ENUM;
    if (typen.find("template") != std::string::npos) type |= DOCTYPE_TEMPLATE;
    if (typen.find("namespace") != std::string::npos) type |= DOCTYPE_NAMESPACE;
    return type;
}

Documentation parseDocumentation(Json::Value data) {
    Documentation retval;
    retval.name = data["name"].asString();
    retval.prototype = data["prototype"].asString();
    retval.type = getType(data["type"].asString());
    return retval;
}

Function parseFunction(Json::Value data) {
    Function retval(parseDocumentation(data));
    for (std::string key : data["parameters"].getMemberNames()) retval.parameters[key] = data["parameters"][key].asString();
    retval.return_value = data["return_value"].asString();
    return retval;
}

Class parseClass(Json::Value data) {
    Class retval(parseDocumentation(data));
    for (Json::Value::ArrayIndex i = 0; i < data["functions"].size(); i++) {
        retval.functions.push_back(data["functions"][i].asString());
    }
    return retval;
}

Enum parseEnum(Json::Value data) {
    Enum retval(parseDocumentation(data));
    for (std::string key : data["constants"].getMemberNames()) retval.constants[key] = data["constants"][key].asString();
    return retval;
}

Json::Value Class::getFunction(std::string func) {
    bool good = false;
    Json::Value root;
    for (std::string val : functions) if (val == func) good = true;
    if (good) {
        if (func == "(constructor)") func = "constructor";
        else if (func == "(destructor)") func = "destructor";	
        std::ifstream in(std::string("docs/" + name + "." + func + ".json").c_str());
        in >> root;
        in.close();
    }
    return root;
}
