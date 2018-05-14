//
//  main.cpp
//  cplusplus
//
//  Created by Jack on 5/9/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#include "docspec.h"
#include "screenio.hpp"

list convertJSON(Json::Value data) {
    if (data.type() != Json::arrayValue) return list();
    list retval;
    for (Json::Value::ArrayIndex i = 0; i < data.size(); i++) retval.push_back(data[i].asString());
    return retval;
}

void showDoc(Json::Value item) {
    list choices;
    choices.push_back("Prototype");
    choices.push_back("Type");
    int type = getType(item["type"].asString());
    if (type & DOCTYPE_FUNCTION) {
        if (item.isMember("parameters") && !item["parameters"].empty()) choices.push_back("Parameters");
        if (item.isMember("return_value")) choices.push_back("Return Value");
    }
    if (type & DOCTYPE_CLASS && item.isMember("functions")) choices.push_back("Methods");
    if (type & DOCTYPE_ENUM && item.isMember("constants")) choices.push_back("Constants");
    while (true) {
        int selection = showMenu(item["name"].asString(), choices);
        if (selection == -1) break;
        else if (selection == 0) {
            setTitle("Prototype");
            setText(item["prototype"].asString());
            while (getButton() != BUTTON_BACK) ;
        } else if (selection == 1) {
            setTitle("Type");
            setText(item["type"].asString());
            while (getButton() != BUTTON_BACK) ;
        } else if (choices[selection] == "Return Value") {
            setTitle("Return Value");
            setText(item["return_value"].asString());
            while (getButton() != BUTTON_BACK) ;
        } else if (choices[selection] == "Constants") {
            list consts = convertJSON(item["constants"]);
            showMenu("Constants", consts);
        } else if (choices[selection] == "Parameters") {
            list params = item["parameters"].getMemberNames();
            while (true) {
                int param = showMenu("Parameters", params);
                if (param != -1) {
                    setTitle(params[param]);
                    setText(item["parameters"][params[param]].asString());
                    while (getButton() != BUTTON_BACK) ;
                } else break;
            }
        } else if (choices[selection] == "Methods") {
            list funcs = convertJSON(item["functions"]);
            while (true) {
                int func = showMenu("Methods", funcs);
                if (func != -1) {
                    Class cl = parseClass(item);
                    showDoc(cl.getFunction(funcs[func]));
                } else break;
            }
        }
    }
}

int main() {
    // insert code here...
    std::ifstream in("headers.json");
    Json::Value root;
    in >> root;
    in.close();
    list categories = root.getMemberNames();
    initialize();
    while (true) {
        int catnum = showMenu("Categories", categories);
        if (catnum != -1) {
            list headers = root[categories[catnum]].getMemberNames();
            while (true) {
                int headnum = showMenu(categories[catnum], headers);
                if (headnum != -1) {
                    list items = convertJSON(root[categories[catnum]][headers[headnum]]);
                    while (true) {
                        int itemnum = showMenu(headers[headnum], items);
                        if (itemnum != -1) {
                            Json::Value item;
                            in.open(std::string("docs/" + headers[headnum] + "." + items[itemnum] + ".json").c_str());
                            in >> item;
                            in.close();
                            showDoc(item);
                        } else break;
                    }
                } else break;
            }
        } else break;
    }
    deinitialize();
    return 0;
}
