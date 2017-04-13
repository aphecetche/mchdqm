#include "AliCDBManager.h"
#include "AliCodeTimer.h"
#include "AliMUONQADataMakerRec.h"
#include "AliRawReader.h"
#include "AliRecoParam.h"
#include <iostream>
#include <memory>
#include <string>
#include "AliQAv1.h"

std::unique_ptr<AliMUONQADataMakerRec> fqadmrec = nullptr;

int Usage()
{
    std::cout << "mchqashifter --ocdb OCDBPATH --input INPUTSPEC" << std::endl;
    return 0;
}

int RunNumber(std::string input)
{
    // find out the run number from the input
    
    auto reader = std::unique_ptr<AliRawReader>(AliRawReader::Create(input.c_str()));

    if (!reader) 
    {
        return -1;
    }

    reader->NextEvent();
    reader->ReadHeader();

    return reader->GetRunNumber();
}

void StartRun(int runNumber)
{
  // Set event species in AliQAv1
  for (unsigned int spec = 0; spec < AliRecoParam::kNSpecies; spec++) {
      AliQAv1::Instance()->SetEventSpecie(AliRecoParam::ConvertIndex(spec));
  }
}

void StartCycle(int runNumber)
{
  fqadmrec->StartOfCycle(AliQAv1::kRAWS,runNumber);
}

void LoopCycle(std::string input)
{
    auto reader = std::unique_ptr<AliRawReader>(AliRawReader::Create(input.c_str()));

    std::cout << "Number of events to be processed: " << reader->GetNumberOfEvents()
         << std::endl;

    while ( reader->NextEvent() )
    {
        fqadmrec->Exec(AliQAv1::kRAWS,reader.get());
    }
}

void EndCycle()

{
  fqadmrec->EndOfCycle(AliQAv1::kRAWS);
}

void EndRun()
{

}

void Setup(std::string ocdbPath, int runNumber)
{
  AliCDBManager* man = AliCDBManager::Instance();
  man->SetDefaultStorage(ocdbPath.c_str());
  man->SetRun(runNumber);
}

int main(int argc, char* argv[]) 
{
    std::string ocdbPath("raw://");
    std::string input;

    for ( int i = 1; i < argc; ++i ) {
        std::cout << "argv[" << i << "]=" << argv[i] << std::endl;

        if (std::string(argv[i])=="--ocdb")
        {
            ocdbPath=argv[i+1];
            ++i;
        }
        if (std::string(argv[i])=="--input")
        {
            input=argv[i+1];
            ++i;
        }
    }
  
    if (input.empty()) {
        std::cerr << "input not specified" << std::endl;
        return Usage();
    }
   
    int runNumber = RunNumber(input);

    if (runNumber < 0 )
    {
        std::cerr << "Could not get the run number for input=" << input << std::endl;
        return Usage();
    }
    
    Setup(ocdbPath,runNumber);

    fqadmrec = std::unique_ptr<AliMUONQADataMakerRec>(new AliMUONQADataMakerRec(true,false));
   
    fqadmrec->Init(AliQAv1::kRAWS,0);

    // fqadmrec->SetEventSpecie(AliRecoParam::kLowMult); // FIXME: get this from somewhere

  // init all species histograms in AliQADataMakerRec
  for (unsigned int es = 0; es < AliRecoParam::kNSpecies; es++) {
    fqadmrec->SetEventSpecie(AliRecoParam::ConvertIndex(es));
    fqadmrec->InitRaws();
  }

  StartRun(runNumber);

  StartCycle(runNumber);

  LoopCycle(input);
  
  EndCycle();

  EndRun();

  AliCodeTimer::Instance()->Print();

  return 0;

}
