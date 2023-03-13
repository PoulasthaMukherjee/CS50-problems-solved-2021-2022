#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        // Check if the voted person is a candidate or not
        if (strcmp(candidates[i], name) == 0)
        {
            // If yes, then update that candidate's rank
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        // Check that the candidate in the current iteration is preferred over the remaining next candidates rank_wise 
        for (int j = i + 1; j < candidate_count; j++)
        {
            // Update the voter's preference of current candidate over other candidates
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        // Check next candidates to make pairs
        for (int j = 0; j < candidate_count; j++)
        {
            // Only make a pair in win-loss situation. Tied pairs are not counted
            if (preferences[i][j] > preferences[j][i])
            {
                // Save who is the winner and loser of a pair
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                // Update the number of valid pairs
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        // Define the position of the highest marginal win
        int max_index = i;
        // Define margin/strength of a win for current iteration/pair
        int current_strength = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];

        // Check the next pairs for greater win margin
        for (int j = i + 1; j < pair_count; j++)
        {
            // Define a temporary strength for checking win margin of the next iterations/pairs
            int temp_strength = preferences[pairs[j].winner][pairs[j].loser] - preferences[pairs[j].loser][pairs[j].winner];
            // Substitute the temporary strength with current strength if temporary is greater than the current 
            if (temp_strength > current_strength)
            {
                max_index = j;
                current_strength = preferences[pairs[j].winner][pairs[j].loser] - preferences[pairs[j].loser][pairs[j].winner];
            }
        }
        // Swap positions of the strongest win to the current max_index 
        pair temp = pairs[i];
        pairs[i] = pairs[max_index];
        pairs[max_index] = temp;
    }
    return;
}

// Check for cyclical pairs
// Define cycle
bool make_cycle(int start, int loser)
{
    // Make cycle if the loser is locked over the start of the cycle i.e., loser = winner condition 
    if (locked[loser][start])
    {
        return true;
    }
    else
    {
        // Check for next candidate in the loop
        for (int i = 0; i < candidate_count; i++)
        {
            // Check if loser of current iteration wins over the next candidate
            if (locked[loser][i])
            {
                // Continue making the cycle
                if (make_cycle(start, i))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!make_cycle(pairs[i].winner, pairs[i].loser))
        {
            // Lock the pairs unless it makes a cycle
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!locked[pairs[i].winner][pairs[i].loser])
        {
            // Winner is the source of the cycle
            // Winner is the loser of the i'th pair which is not locked
            printf("%s\n", candidates[i]);
        }
    }
    return;
}