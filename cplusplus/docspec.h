//
//  docspec.h
//  cplusplus
//
//  Created by Jack on 5/9/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#ifndef docspec_h
#define docspec_h

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <json/json.h>

enum doc_type {
    DOCTYPE_UNKNOWN = 0,
    DOCTYPE_TEMPLATE = 1,
    DOCTYPE_FUNCTION = 2,
    DOCTYPE_CLASS = 4,
    DOCTYPE_MACRO = 8,
    DOCTYPE_ENUM = 16,
    DOCTYPE_NAMESPACE = 32
};

class Documentation {
    public:
    std::string name;
    std::string prototype;
    int type;
};

class Function : public Documentation {
    public:
    std::map<std::string, std::string> parameters;
    std::string return_value;
    Function(Documentation base) {
        name = base.name;
        prototype = base.prototype;
        type = base.type;
    }
};

class Class : public Documentation {
    public:
    std::vector<std::string> functions;
    Json::Value getFunction(std::string func);
    Class(Documentation base) {
        name = base.name;
        prototype = base.prototype;
        type = base.type;
    }
};

class Enum : public Documentation {
    public:
    std::map<std::string, std::string> constants;
    Enum(Documentation base) {
        name = base.name;
        prototype = base.prototype;
        type = base.type;
    }
};

extern int getType(std::string typen);
extern Documentation parseDocumentation(Json::Value data);
extern Function parseFunction(Json::Value data);
extern Class parseClass(Json::Value data);
extern Enum parseEnum(Json::Value data);

#endif /* docspec_h */
