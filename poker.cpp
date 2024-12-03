#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <string>

using namespace std;

// 카드 모양을 이모지로 정의
const vector<string> SUITS = {"♥", "♦", "♣", "♠"};
const vector<string> RANKS = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

struct Card {
    string rank;
    string suit;

    string to_string() const {
        return rank + suit;
    }
};

class Deck {
private:
    vector<Card> cards;

public:
    Deck() {
        for (const string& suit : SUITS) {
            for (const string& rank : RANKS) {
                cards.push_back({rank, suit});
            }
        }
    }

    void shuffle() {
        random_device rd;
        mt19937 g(rd());
        std::shuffle(cards.begin(), cards.end(), g);
    }

    Card deal() {
        if (cards.empty()) {
            throw runtime_error("덱이 비어 있습니다!");
        }
        Card top_card = cards.back();
        cards.pop_back();
        return top_card;
    }
};

class TexasHoldem {
private:
    Deck deck;
    vector<Card> player_hand;
    vector<Card> ai_hand;
    vector<Card> community_cards;
    int player_chips;
    int ai_chips;
    int pot = 0;
    int initial_player_chips;
    int initial_ai_chips;

    void deal_initial_hands() {
        player_hand.clear();
        ai_hand.clear();
        community_cards.clear();
        deck = Deck();
        deck.shuffle();

        for (int i = 0; i < 2; ++i) {
            player_hand.push_back(deck.deal());
            ai_hand.push_back(deck.deal());
        }
    }

    void deal_community_cards(int count) {
        for (int i = 0; i < count; ++i) {
            community_cards.push_back(deck.deal());
        }
    }

    void show_hand(const vector<Card>& hand) const {
        for (const Card& card : hand) {
            cout << card.to_string() << " ";
        }
        cout << endl;
    }

    void player_action() {
        cout << "현재 칩: " << player_chips << ", 팟: " << pot << endl;
        cout << "선택하세요: 1) 콜  2) 레이즈  3) 폴드" << endl;
        int choice;
        cin >> choice;

        if (choice == 1) { // 콜
            cout << "플레이어가 콜했습니다." << endl;
            pot += 100;
            player_chips -= 100;
        } else if (choice == 2) { // 레이즈
            cout << "얼마를 베팅하시겠습니까? ";
            int raise_amount;
            cin >> raise_amount;
            if (raise_amount > player_chips) {
                cout << "베팅 금액이 칩보다 많습니다. 다시 입력하세요." << endl;
                player_action();
                return;
            }
            cout << "플레이어가 " << raise_amount << "만큼 레이즈했습니다." << endl;
            pot += raise_amount;
            player_chips -= raise_amount;
        } else if (choice == 3) { // 폴드
            cout << "플레이어가 폴드했습니다. AI가 승리합니다." << endl;
            ai_chips += pot;
            pot = 0;
            return;
        } else {
            cout << "잘못된 입력입니다. 다시 선택하세요." << endl;
            player_action();
        }
    }

    bool ai_action() {
        int action = rand() % 3; // AI 행동 결정 (0: 콜, 1: 레이즈, 2: 폴드)
        if (action == 0) { // 콜
            cout << "AI가 콜했습니다." << endl;
            pot += 100;
            ai_chips -= 100;
        } else if (action == 1) { // 레이즈
            int raise_amount = 100 + rand() % 200; // 랜덤 베팅
            if (raise_amount > ai_chips) {
                raise_amount = ai_chips; // 남은 칩만큼만 베팅
            }
            cout << "AI가 " << raise_amount << "만큼 레이즈했습니다." << endl;
            pot += raise_amount;
            ai_chips -= raise_amount;
        } else if (action == 2) { // 폴드
            cout << "AI가 폴드했습니다. 플레이어가 승리합니다." << endl;
            player_chips += pot;
            pot = 0;
            return true; // AI가 폴드했음을 알림
        }
        return false; // 계속 진행
    }

    string evaluate_hand(const vector<Card>& hand) {
        // 핸드 평가 로직 (위 코드에서 복사 가능)
        return "하이카드"; // 임시로 하이카드 반환
    }

    void determine_winner() {
        vector<Card> player_full_hand = player_hand;
        vector<Card> ai_full_hand = ai_hand;

        player_full_hand.insert(player_full_hand.end(), community_cards.begin(), community_cards.end());
        ai_full_hand.insert(ai_full_hand.end(), community_cards.begin(), community_cards.end());

        string player_result = evaluate_hand(player_full_hand);
        string ai_result = evaluate_hand(ai_full_hand);

        cout << "\n플레이어의 카드 족보: " << player_result << endl;
        cout << "AI의 카드 족보: " << ai_result << endl;

        if (player_result == ai_result) {
            cout << "\n무승부입니다! 팟이 유지됩니다." << endl;
        } else {
            cout << "\n플레이어가 승리했습니다! (" << player_result << ")" << endl;
            player_chips += pot;
        }

        // 칩 상태 출력
        cout << "\n게임 종료 후 칩 상태:\n";
        cout << "플레이어 칩: " << player_chips << endl;
        cout << "AI 칩: " << ai_chips << endl;

        pot = 0;
    }

    bool ask_to_play_again() {
        cout << "\n한 번 더 플레이하시겠습니까? (1: 예, 0: 아니오): ";
        int choice;
        cin >> choice;
        return choice == 1;
    }

    void show_final_result() {
        int total_gain = player_chips - initial_player_chips;
        cout << "\n최종 칩 상태:\n";
        cout << "플레이어 칩: " << player_chips << " (처음 칩: " << initial_player_chips << ")" << endl;

        if (total_gain > 0) {
            cout << "축하합니다! 총 " << total_gain << " 칩을 얻었습니다.\n";
        } else if (total_gain < 0) {
            cout << "안타깝게도 총 " << -total_gain << " 칩을 잃었습니다.\n";
        } else {
            cout << "칩 변동이 없습니다.\n";
        }
    }

public:
    TexasHoldem() : player_chips(1000), ai_chips(1000), initial_player_chips(1000), initial_ai_chips(1000) {}

    void play() {
        do {
            deal_initial_hands();
            cout << "\n플레이어의 카드: ";
            show_hand(player_hand);

            // 플랍
            cout << "\n플랍 단계" << endl;
            deal_community_cards(3);
            show_hand(community_cards);
            player_action();
            if (ai_action()) continue; // AI가 폴드하면 다음 게임으로

            // 턴
            cout << "\n턴 단계" << endl;
            deal_community_cards(1);
            show_hand(community_cards);
            player_action();
            if (ai_action()) continue; // AI가 폴드하면 다음 게임으로

            // 리버
            cout << "\n리버 단계" << endl;
            deal_community_cards(1);
            show_hand(community_cards);
            player_action();
            if (ai_action()) continue; // AI가 폴드하면 다음 게임으로

            // 승자 결정
            determine_winner();
        } while (ask_to_play_again());

        // 최종 결과 출력
        show_final_result();
    }
};

int main() {
    cout << "텍사스 홀덤 포커 게임에 오신 것을 환영합니다!\n" << endl;
    TexasHoldem game;
    game.play();
    return 0;
}
