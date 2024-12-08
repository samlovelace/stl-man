#ifndef PARSER_H
#define PARSER_H

#include <string> 
#include <chrono>
#include <vector>
#include <map>
#include "StlModel.h"

class Parser 
{ 
public:
    Parser();
    ~Parser();

    bool parse(std::string aFilename); 
    float tookHowLong() {return mParseTime.count(); }
    STLModel& getModel(std::string aModelFilename);

private:
    std::chrono::duration<float> mParseTime; 

    Chunk parseChunk(std::vector<std::string> aChunk); 
    Vector3 parseNormalLine(const std::string& aLine); 
    Vector3 parseVertexLine(const std::string& aLine); 

    std::vector<std::string> splitStringBySpace(const std::string& input);

    std::map<std::string, STLModel> mModels; 
};
#endif //PARSER_H