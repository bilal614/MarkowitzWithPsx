#pragma once
#include <iostream>
#include <fstream>
#include <memory>
#include <initializer_list>

namespace MeanVarianceFrontier
{
    class CsvWriter 
    {
    public:
        CsvWriter(std::string filePathName, std::initializer_list<std::string> columnNames) :
            m_file{ std::make_unique<std::ofstream>(std::ofstream{ filePathName, std::ios::out }) }
        {
            if (*m_file)
            {
                for(auto colName = columnNames.begin(); colName != columnNames.end(); ++colName)
                {
                    (*m_file) << *colName << ((colName != (columnNames.end() - 1)) ? "," : "\n");
                }
            }
        }

        template<typename... Str>
        void writeLine(Str... args);
    private:
        std::unique_ptr<std::ofstream> m_file;
    };

    template<typename ...Str>
    inline void CsvWriter::writeLine(Str ...args)
    {
        std::vector<std::string> lineArgs{ args... }; //unpack ints here
        for (auto arg = lineArgs.begin(); arg != lineArgs.end(); ++arg)
            *m_file << (*arg) << ((arg != (lineArgs.end() - 1)) ? "," : "\n");
    }
}