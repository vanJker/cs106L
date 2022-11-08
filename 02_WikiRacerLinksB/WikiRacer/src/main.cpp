
#include <iostream>
#include <unordered_set>
#include <vector>
#include <queue>
#include <unordered_map>
#include "wikiscraper.h"


using std::cout;            using std::endl;
using std::string;          using std::vector;
using std::priority_queue;  using std::unordered_map;
using std::unordered_set;




// Return the intersection of two (unordered) set.
template <typename T>
unordered_set<T> unordered_set_intersection(const unordered_set<T>& set1,
                                            const unordered_set<T>& set2) {
    unordered_set<T> intersection_set;
    for (T x: set1) {
        if (set2.count(x)) intersection_set.insert(x);
    }
    return intersection_set;
}

// Print ladder as format.
template <typename T>
void printLadder(const vector<T>& ladder, const string& mid,
                 const string& beg, const string& end) {
    cout << beg;
    for (size_t i = 0; i < ladder.size(); i++) {
        cout << ladder.at(i);
        if (i < ladder.size() - 1) cout << mid;
    }
    cout << end << endl;
}


/*
 * This is the function you will be implementing. It takes
 * two string representing the names of a start_page and
 * end_page and is supposed to return a ladder, represented
 * as a vector<string>, of links that can be followed from
 * start_page to get to the end_page.
 *
 * For the purposes of this algorithm, the "name" of a Wikipedia
 * page is what shows at the end of the URL when you visit that page
 * in your web browser. For ex. the name of the Stanford University
 * Wikipedia page is "Stanford_University" since the URL that shows
 * in your browser when you visit this page is:
 *
 *       https://en.wikipedia.org/wiki/Stanford_University
 */
vector<string> findWikiLadder(const string& start_page, const string& end_page) {
    // TODO: Fill in the findWikiLinks method in wikiscraper.cpp,
    //       then write this  function as per the handout.
    //
    //                Best of luck!

    // creates WikiScraper object
    WikiScraper scraper;

    // gets the set of links on page specified by end_page
    // variable and stores in target_set variable
    auto target_set = scraper.getLinkSet(end_page);

    // Comparison function for priority_queue
    auto cmpFn = [&scraper, &target_set] (const vector<string>& ladder1,
                                        const vector<string>& ladder2) {
        // word at the enod of ladder1 and ladder2
        auto page1 = ladder1.back();
        auto page2 = ladder2.back();

        // set of links in page1 and page2
        auto page1_set = scraper.getLinkSet(page1);
        auto page2_set = scraper.getLinkSet(page2);

        // number of links in common between set of links on page1/page2
        // and set of links on end_page
        int num1 = unordered_set_intersection(page1_set, target_set).size();
        int num2 = unordered_set_intersection(page2_set, target_set).size();

        return num1 < num2;
    };

    // creates a priority_queue names ladderQueue
    std::priority_queue<vector<string>, vector<vector<string>>,
                        decltype(cmpFn)> ladderQueue(cmpFn);

    // create a ladder containing {start_page} to the queue
    vector<string> ladder;
    ladder.push_back(start_page);
    ladderQueue.push(ladder);

    // create a set stores visited pages
    unordered_set<string> visited;

    while (!ladderQueue.empty()) {
        // dequeue the highest priority partial-ladder
        vector<string> pladder = ladderQueue.top();
        ladderQueue.pop();

        // print the ladder
        printLadder(pladder, ", ", "{", "}");

        // get the set of links of current page
        // and add the page to visited set
        auto page = ladder.back();
        auto page_set = scraper.getLinkSet(page);
        visited.insert(page);

        if (page_set.count(end_page)) {
            pladder.push_back(end_page);
            return pladder;
        }

        for (auto page: page_set) {
            if (visited.count(page) == 0) {
                vector<string> nladder(pladder);
                nladder.push_back(page);
                ladderQueue.push(nladder);
            }
        }
    }

    return {};
}



int main() {
    auto ladder = findWikiLadder("Fruit", "Strawberry");
    cout << endl;

    if(ladder.empty()) {
        cout << "No ladder found!" << endl;
    } else {
        cout << "Ladder found:" << endl;
        cout << "\t";

        // Print the ladder here!
        printLadder(ladder, " -> ", "", "");
    }

    return 0;
}




