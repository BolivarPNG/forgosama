#include "request_queue.h"

RequestQueue::RequestQueue(const SearchServer& search_server)
    : search_server_(search_server)
    , no_data_requests_(0)
    , time_(0) 
{
}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentStatus status) 
{
    const auto result = search_server_.FindTopDocuments(raw_query, status);
    DocAdd(result.size());
    return result;
}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query) 
{
    const auto result = search_server_.FindTopDocuments(raw_query);
    DocAdd(result.size());
    return result;
}

int RequestQueue::GetNoResultRequests() const 
{
    return no_data_requests_;
}

void RequestQueue::DocAdd(int results_num) 
{
    // новый запрос - новая секунда
    ++time_;
    // удаляем все результаты поиска, которые устарели
    while (!requests_.empty() && min_in_day_ <= time_ - requests_.front().time_req) 
    {
        if (0 == requests_.front().results) 
        {
            --no_data_requests_;
        }
        requests_.pop_front();
    }
    // сохраняем новый результат поиска
    requests_.push_back({ time_, results_num });
    if (0 == results_num) 
    {
        ++no_data_requests_;
    }
}