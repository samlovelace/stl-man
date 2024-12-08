#include "Parser.h"
#include <iostream>
#include <fstream> 
#include <sstream> 

#include "StlModel.h"

Parser::Parser()
{

}

Parser::~Parser()
{

}

bool Parser::parse(std::string aFilename)
{
    // Start timer here
    auto start = std::chrono::high_resolution_clock::now(); 

    // open the file 
    std::ifstream file(aFilename);
    if (!file) {
        std::cerr << "Error: Could not open file " << aFilename << std::endl;
        return false;
    }

    STLModel model; 
    Chunk chunk; 
    int chunkSize = 6; 
    std::vector<std::string> chunkLines; 


    // parse each line
    std::string line;
    while (std::getline(file, line)) 
    {
        if(line.find("facet normal") != std::string::npos)
        {
            chunkLines.push_back(line); 

            for(int i = 0; i < chunkSize && std::getline(file, line); i++)
            {
                chunkLines.push_back(line);   
            }

            chunk = parseChunk(chunkLines); 
            model.chunks.push_back(chunk); 
        }

        chunkLines.clear();  
    }

    file.close();

    auto end = std::chrono::high_resolution_clock::now(); 
    mParseTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    mModels[aFilename] = model; 

    return true; 
}

Chunk Parser::parseChunk(std::vector<std::string> aChunk)
{
    Chunk chunk; 
    Vector3 normal = parseNormalLine(aChunk[0]);
    chunk.normal = normal; 

    for(int j = 0; j < 3; j++)
    {
        Vector3 vertex = parseVertexLine(aChunk[j+2]);
        chunk.vertices.push_back(vertex); 
    }

    return chunk; 
}

Vector3 Parser::parseNormalLine(const std::string& aLine)
{
    auto words = splitStringBySpace(aLine); 

    Vector3 normal; 
    normal.x = std::stof(words[2]);
    normal.y = std::stof(words[3]); 
    normal.z = std::stof(words[4]); 

    return normal; 
}

Vector3 Parser::parseVertexLine(const std::string& aLine)
{
    auto words = splitStringBySpace(aLine); 

    Vector3 vertex; 
    vertex.x = std::stof(words[1]);
    vertex.y = std::stof(words[2]);
    vertex.z = std::stof(words[3]);

    return vertex; 
}

std::vector<std::string> Parser::splitStringBySpace(const std::string& input) {
    std::istringstream stream(input);
    std::vector<std::string> words;
    std::string word;

    // Extract words separated by spaces
    while (stream >> word) {
        words.push_back(word);
    }

    return words;
}

STLModel& Parser::getModel(std::string aModelFilename)
{
    if(mModels.find(aModelFilename) != mModels.end())
    {
        std::cout << "Retrieving model: " << aModelFilename << std::endl; 
        return mModels[aModelFilename]; 
    }
    else{
        std::cerr << "Couldnt not find parsed model for " << aModelFilename << std::endl; 
    }
}

