#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Version sans vector : les deux sequences sont des chaines de caracteres.
bool estSousSequenceSansVector(const string& A, const string& C)
{
	if (C.size() > A.size())
	{
		return false;
	}

	string::size_type indiceC = 0;
	for (string::size_type indiceA = 0;
		 indiceA < A.size() && indiceC < C.size(); ++indiceA)
	{
		if (A[indiceA] == C[indiceC])
		{
			++indiceC;
		}
	}

	return indiceC == C.size();
}

// Version avec vector<char> : chaque caractere occupe une case du vector.
bool estSousSequenceAvecVector(const vector<char>& A, const vector<char>& C)
{
	if (C.size() > A.size())
	{
		return false;
	}

	vector<char>::size_type indiceC = 0;
	for (vector<char>::size_type indiceA = 0;
		 indiceA < A.size() && indiceC < C.size(); ++indiceA)
	{
		if (A[indiceA] == C[indiceC])
		{
			++indiceC;
		}
	}

	return indiceC == C.size();
}

int main()
{
	const string sequenceA = "abacabad";
	const string sequenceC = "acd";
	const vector<char> sequenceAVector(sequenceA.begin(), sequenceA.end());
	const vector<char> sequenceCVector(sequenceC.begin(), sequenceC.end());

	cout << "Avec string      : "
		 << (estSousSequenceSansVector(sequenceA, sequenceC) ? "oui" : "non")
		 << '\n';
	cout << "Avec vector<char>: "
		 << (estSousSequenceAvecVector(sequenceAVector, sequenceCVector)
				 ? "oui" : "non")
		 << '\n';

	const string sequenceInvalide = "dba";
	const vector<char> sequenceInvalideVector(sequenceInvalide.begin(),
												 sequenceInvalide.end());
	cout << "Ordre incorrect : "
		 << (estSousSequenceSansVector(sequenceA, sequenceInvalide) ? "oui" : "non")
		 << " / "
		 << (estSousSequenceAvecVector(sequenceAVector, sequenceInvalideVector)
				 ? "oui" : "non")
		 << '\n';

	const string sequenceVide;
	const vector<char> sequenceVideVector;
	cout << "Sequence vide   : "
		 << (estSousSequenceSansVector(sequenceA, sequenceVide) ? "oui" : "non")
		 << " / "
		 << (estSousSequenceAvecVector(sequenceAVector, sequenceVideVector)
				 ? "oui" : "non")
		 << '\n';

	return 0;
}
