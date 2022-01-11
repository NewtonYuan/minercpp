#include <picosha2.h>
#include <taskflow.hpp>
#include <sstream>
#include "json.hpp"
#include <sys/stat.h>
using json = nlohmann::json;

int jsonSize = 0;
bool running = true;
std::string prefix = "0";
std::vector<int> inc{0, 1, 2, 3, 4, 5, 6, 7};

void miner(uint64_t nonceNum) {
    std::string source; source = "block41";
    std::string data; data = "somethingsomethingsomething";
    std::string target; target = "21e80";
    std::string nonce; 
    std::string resultHashStr;
    std::string sourceHashStr;
    std::string dataHashStr;
    std::vector<unsigned char> sourceHash(picosha2::k_digest_size);
    picosha2::hash256(source.begin(), source.end(), sourceHash.begin(), sourceHash.end());

    std::vector<unsigned char> dataHash(picosha2::k_digest_size);
    picosha2::hash256(data.begin(), data.end(), dataHash.begin(), dataHash.end());

    sourceHashStr = picosha2::bytes_to_hex_string(sourceHash.begin(), sourceHash.end());
    dataHashStr = picosha2::bytes_to_hex_string(dataHash.begin(), dataHash.end());

    nonceNum += inc.front();
    inc.erase(inc.begin());

    std::cout << std::endl << nonceNum << std::endl;

    while ( prefix != target && running == true ) {

        std::stringstream ss; ss << nonceNum; ss >> nonce;

        std::string hashStr = sourceHashStr + dataHashStr + nonce;

        std::vector<unsigned char> hash(picosha2::k_digest_size);

        picosha2::hash256(hashStr.begin(), hashStr.end(), hash.begin(), hash.end());

        resultHashStr = picosha2::bytes_to_hex_string(hash.begin(), hash.end());

        prefix = resultHashStr.substr(0, 5);

        nonceNum = nonceNum + 8;
    }
    
    /*std::ostringstream oss; oss << "hashes/" << sourceHashStr << ".json";
    std::string hashFile = oss.str();

    json j;
    try {
        std::ifstream i(hashFile);
        i >> j;
    } catch (json::parse_error& ex){}

    j[resultHashStr] = {
        {"source", source},
        {"data", data},
        {"sourceHash", sourceHashStr},
        {"dataHash", dataHashStr},
        {"nonce", nonce}};

    jsonSize = j.size();
    std::cout << jsonSize << std::endl;

    std::ofstream o(hashFile);
    o << std::setw(4) << j << std::endl;*/

    std::cout << std::endl << resultHashStr << std::endl;

    running = false;
    prefix = target;
};

void testing(uint64_t hello){
    std::cout << hello << std::endl;
}

int main(){
    tf::Executor executor;
    tf::Taskflow taskflow;
    
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<long long unsigned> distribution(0,0xFFFFFFFFFFFFFFFF);
    uint64_t nonceNum = distribution(generator);
    uint64_t starter = 0;
    uint64_t result = 100;
    uint64_t steps = 1;

    taskflow.for_each_index(0, 8, 1, [nonceNum](uint64_t i) {miner(nonceNum);});  // 50 loops with a + step
    /*auto Miner1 = taskflow.emplace([&](){miner(nonceNum);}).name("Miner 1");
    nonceNum = nonceNum + 1;
    auto Miner2 = taskflow.emplace([&](){miner(nonceNum);}).name("Miner 2");
    nonceNum = nonceNum + 1;
    auto Miner3 = taskflow.emplace([&](){miner(nonceNum);}).name("Miner 3");
    nonceNum = nonceNum + 1;
    auto Miner4 = taskflow.emplace([&](){miner(nonceNum);}).name("Miner 4");
    nonceNum = nonceNum + 1;
    auto Miner5 = taskflow.emplace([&](){miner(nonceNum);}).name("Miner 5");
    nonceNum = nonceNum + 1;
    auto Miner6 = taskflow.emplace([&](){miner(nonceNum);}).name("Miner 6");
    nonceNum = nonceNum + 1;
    auto Miner7 = taskflow.emplace([&](){miner(nonceNum);}).name("Miner 7");*/
    executor.run(taskflow).wait();
    //miner(nonceNum);
}