# Reconnaissance d'une sous-sequence

Ce dossier implemente la reconnaissance d'une sous-sequence de caracteres. Le programme verifie si une chaine `C` est une sous-sequence d'une chaine `A` avec deux representations : `std::string` sans `vector`, puis `vector<char>`.

## 1. Definition

Une sequence `C` est une sous-sequence de `A` si l'on peut supprimer zero ou plusieurs elements de `A` et obtenir `C`, sans changer l'ordre des elements conserves. Les elements n'ont pas besoin d'etre cote a cote.

Exemple : `A = "abacabad"` contient `C = "acd"` comme sous-sequence : on choisit successivement `a`, `c`, puis `d`, en positions 1, 4 et 8. En revanche, `"dba"` n'est pas une sous-sequence : apres avoir trouve `d` a la fin de `A`, il ne reste plus de `b` a choisir.

Une sous-sequence n'est donc pas la meme chose qu'une sous-chaine contigue. Par exemple, `"acd"` n'est pas un bloc contigu de `A`, mais c'est bien une sous-sequence.

La sequence vide est une sous-sequence de toute sequence. Une sequence plus longue que `A` ne peut pas etre une sous-sequence de `A`.

## 2. Idee de l'algorithme

On parcourt `A` de gauche a droite et on garde un indice vers le prochain element de `C` a trouver.

1. Initialiser `indiceC` a zero : aucun element de `C` n'a encore ete trouve.
2. Lire chaque `A[indiceA]` dans l'ordre.
3. Si cet element est egal a `C[indiceC]`, avancer `indiceC`. Cet element est alors reconnu et on cherchera le suivant de `C`.
4. Sinon, ne pas avancer `indiceC` : l'element courant de `A` ne sert pas, mais pourra etre ignore.
5. A la fin, `C` est une sous-sequence exactement si `indiceC` a atteint la taille de `C`.

On ne revient jamais en arriere. C'est essentiel : cela impose que les correspondances soient trouvees dans le meme ordre que dans les sequences originales.

### Invariant de boucle

Au debut de chaque tour, `indiceC` est le nombre d'elements initiaux de `C` deja reconnus, dans le bon ordre, parmi les elements de `A` deja parcourus. Une egalite consomme exactement le prochain element attendu de `C`; une absence d'egalite ne change pas le prefixe reconnu. A la sortie, comparer `indiceC` a la taille de `C` donne donc la bonne reponse.

## 3. Version sans `vector`

Fonction : `estSousSequenceSansVector` dans [sousSeq.cpp](sousSeq.cpp). Ici, « sans vector » signifie que la chaine est stockee dans `std::string`, pas que la memoire est geree manuellement.

Elle recoit `A` et `C` par reference constante. `size()` fournit leurs longueurs, les caracteres sont accessibles par indice et la fonction ne modifie pas les chaines. Le test rapide `C.size() > A.size()` elimine un cas impossible avant la boucle.

La boucle utilise `indiceA` pour avancer dans `A` et `indiceC` pour suivre la progression dans `C`. Le type `string::size_type` convient aux tailles et indices de chaine. Si `C` est vide, la condition de boucle est immediatement fausse et la fonction renvoie `true` sans acceder a un caractere.

Cette approche illustre le traitement direct d'une chaine de caracteres sans conteneur `vector`.

## 4. Version avec `vector`

Fonction : `estSousSequenceAvecVector` dans [sousSeq.cpp](sousSeq.cpp).

L'algorithme est le meme, mais chaque caractere est stocke dans une case de `vector<char>`. Les tailles viennent de `A.size()` et `C.size()`, et le type d'indice est `vector<char>::size_type`. Les parametres sont des references constantes (`const vector<char>&`) : on evite de copier les sequences et la fonction n'a pas le droit de les modifier.

`vector` gere la memoire et les tailles. Il ne change pas la logique ni la complexite de reconnaissance. `std::string` est deja une classe de chaine et n'est pas un tableau C classique.

## 5. Exemple pas a pas

Pour `A = "abacabad"` et `C = "acd"` :

| Caractere lu dans A | Caractere attendu dans C | Action |
|---|---|---|
| a | a | Trouve a, avancer dans C |
| b | c | Ignorer b |
| a | c | Ignorer a |
| c | c | Trouve c, avancer dans C |
| a, b, a | d | Ignorer ces caracteres |
| d | d | Trouve d, avancer dans C |

Tous les elements de `C` ont ete trouves : le resultat est `true`.

Avec `C = "dba"`, la fonction trouve d'abord `d`, mais ne trouve ensuite ni `b` ni `a` dans le reste de `A`. Le resultat est `false`.

## 6. Complexite

Notons `n` la taille de `A` et `m` la taille de `C`.

- **Temps au pire : `O(n)`**. Chaque element de `A` est examine au plus une fois. La comparaison et l'incrementation sont en temps constant. Le test des tailles est aussi constant.
- **Temps au mieux : `O(1)`** dans les cas arretes par le test `m > n`, ou lorsque `C` est vide. Pour les autres cas, la boucle peut s'arreter tot si tous les elements de `C` sont trouves tot; sa borne generale reste `O(n)`.
- **Memoire auxiliaire : `O(1)`**. Seuls deux indices sont utilises. Les sequences fournies en entree ne sont pas comptees comme memoire auxiliaire et ne sont pas copiees par les fonctions.

Le temps ne depend pas de `m` comme facteur multiplicatif : nous ne relancons pas une recherche dans `A` depuis le debut pour chaque element de `C`. Le parcours avance toujours.

## 7. Si la sequence grandit

Les fonctions ci-dessus effectuent une seule verification. Elles ne changent pas `A` ni `C`.

### Ajouter a la fin de A

Ajouter des elements a la fin de `A` ne peut pas detruire une correspondance deja trouvee : une reponse `oui` reste `oui`. Une reponse `non` peut devenir `oui` si les nouveaux elements completent `C` dans le bon ordre.

Si `A` passe de `n` a `n + k` elements, une nouvelle verification prend au pire `O(n + k)`. La memoire necessaire pour stocker `A` augmente egalement de `k` elements.

### Ajouter a la fin de C

Ajouter des elements a `C` rend la condition plus exigeante. Une reponse `oui` pour l'ancien `C` peut devenir `non` pour le nouveau. Une reponse `non` ne peut pas devenir `oui` en ajoutant encore des elements a `C`.

### Ajouter un caractere a `std::string` ou `vector<char>`

On peut ajouter a la fin avec `push_back`. Pour ces conteneurs, cette operation coute `O(1)` amorti : la plupart des ajouts sont rapides, mais un ajout occasionnel peut reallouer le stockage et recopier les caracteres, ce qui coute `O(n)`. Sur `k` ajouts successifs, le cout total est `O(k)` amorti, hors cout initial de construction. `capacity()` indique la place reservee et `size()` le nombre de caracteres presents.

Inserer au milieu de ces conteneurs peut decaler les caracteres suivants et coute `O(n)`; ajouter a la fin est le cas favorable. Une reallocation peut invalider les pointeurs, references et iterateurs vers les caracteres. Dans nos fonctions, aucun caractere n'est ajoute pendant le parcours, donc ce risque ne se presente pas.

### Recalculer apres chaque ajout

Si on ajoute un element puis relance la reconnaissance depuis le debut a chaque fois, avec des tailles successives de `1` a `N`, le cout cumule au pire vaut `1 + 2 + ... + N`, soit `O(N^2)`. Il est donc plus efficace, lorsque c'est possible, d'attendre d'avoir fini les ajouts et de faire une seule verification en `O(N)`.

## 8. Si les sequences sont des chaines de caracteres

La version `std::string` du programme traite directement une chaine. La version `vector<char>` represente la meme sequence avec un conteneur generique de caracteres.

Attention : une chaine UTF-8 peut encoder un caractere visible avec plusieurs octets. Un parcours direct de `std::string` traite les octets, pas necessairement les lettres Unicode. Pour le texte ASCII ou les caracteres simples de l'exercice, cela convient; pour le texte multilingue, il faut d'abord parcourir des points de code Unicode ou des graphèmes avec une bibliotheque adaptee.

L'agrandissement d'une `std::string` est analogue a celui d'un `vector`: `push_back` a la fin est `O(1)` amorti, une reallocation occasionnelle peut copier toute la chaine en `O(n)`, et les references/pointeurs/iterateurs peuvent alors etre invalides. Ces couts de construction sont distincts du cout `O(n)` de l'algorithme de reconnaissance.

## 9. Compilation et execution

Depuis ce dossier :

```sh
g++ -std=c++11 -Wall -Wextra -pedantic sousSeq.cpp -o sousSeq
./sousSeq
```

Sortie attendue :

```text
Avec string      : oui
Avec vector<char>: oui
Ordre incorrect : non / non
Sequence vide   : oui / oui
```
