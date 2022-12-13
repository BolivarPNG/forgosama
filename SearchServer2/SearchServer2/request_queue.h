#pragma once
#include "search_server.h"
#include <deque>

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server);

    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) 
    {
        const auto result = search_server_.FindTopDocuments(raw_query, document_predicate);
        DocAdd(result.size());
        return result;
    }
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);

    std::vector<Document> AddFindRequest(const std::string& raw_query);

    int GetNoResultRequests() const;

private:
    struct QueryResult 
    {
        uint64_t time_req;
        int results;
    };
    std::deque<QueryResult> requests_;
    const SearchServer& search_server_;
    int no_data_requests_;
    uint64_t time_;
    const static int min_in_day_ = 1440;

    void DocAdd(int results_num);
};
