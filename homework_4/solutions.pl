%%%%% Question 1 %%%%%
parent(bart,homer).
parent(bart,marge).
parent(lisa,homer).
parent(lisa,marge).
parent(maggie,homer).
parent(maggie,marge).
parent(homer,abraham).
parent(herb,abraham).
parent(tod,ned).
parent(rod,ned).
parent(marge,jackie).
parent(patty,jackie).
parent(selma,jackie).
female(maggie).
female(lisa).
female(marge).
female(patty).
female(selma).
female(jackie).
male(bart).
male(homer).
male(herb).
male(burns).
male(smithers).
male(tod).
male(rod).
male(ned).
male(abraham).

brother(X, Y) :- male(X),parent(X, P),parent(Y, P),X \= Y.

sister(X, Y) :- female(X),parent(X, P),parent(Y, P),X \= Y.

aunt(X, Y) :- female(Y),parent(X, P),sister(P, Y).

uncle(X, Y) :- male(Y),parent(X, P),brother(P, Y).

grandfather(X, Y) :- male(Y),parent(X, P),parent(P, Y).

granddaughter(X, Y) :- female(Y),parent(P, X),parent(Y, P).

ancestor(X, Y) :- parent(X, Y).

ancestor(X, Y) :- parent(X, Z),ancestor(Z, Y).

%%%%% Question 2 %%%%%
surgeon(X) :- occupation(X, oral_surgeon).
surgeon(X) :- occupation(X, plastic_surgeon).
surgeon(X) :- occupation(X, heart_surgeon).
surgeon(X) :- occupation(X, brain_surgeon).

lawyer(X) :- occupation(X, patent_lawyer).
lawyer(X) :- occupation(X, trial_lawyer).
lawyer(X) :- occupation(X, civil_lawyer).

query2a(X) :- lawyer(X).
query2b(X) :- surgeon(X), address(X, los_angeles).
query2c(X) :- surgeon(X), address(X, dallas), salary(X, Salary), Salary > 100000.
query2c(X) :- surgeon(X), address(X, houston), salary(X, Salary), Salary > 100000.
query2c(X) :- surgeon(X), address(X, college_station), salary(X, Salary), Salary > 100000.

occupation(joe,oral_surgeon).
occupation(sam,patent_lawyer).
occupation(bill,trial_lawyer).
occupation(cindy,investment_banker).
occupation(joan,civil_lawyer).
occupation(len,plastic_surgeon).
occupation(lance,heart_surgeon).
occupation(frank,brain_surgeon).
occupation(charlie,plastic_surgeon).
occupation(lisa,oral_surgeon).
address(joe,houston).
address(sam,pittsburgh).
address(bill,dallas).
address(cindy,omaha).
address(joan,chicago).
address(len,college_station).
address(lance,los_angeles).
address(frank,dallas).
address(charlie,houston).
address(lisa,san_antonio).
salary(joe,50000).
salary(sam,150000).
salary(bill,200000).
salary(cindy,140000).
salary(joan,80000).
salary(len,70000).
salary(lance,650000).
salary(frank,85000).
salary(charlie,120000).
salary(lisa,190000).


%%%%% Question 3 %%%%%

subject(algebra, math).
subject(calculus, math).
subject(dynamics, physics).
subject(electromagnetism, physics).
subject(nuclear, physics).
subject(organic, chemistry).
subject(inorganic, chemistry).

degree(bill, phd, chemistry).
degree(john, bs, math).
degree(chuck, ms, physics).
degree(susan, phd, math).

retired(bill).

canTeach(X, Y) :- degree(X, phd, Z), subject(Y, Z), \+ retired(X).
canTeach2(X, Y) :- canTeach(X, Y), \+ retired(X).
canTeach3(X, Y) :- (degree(X, phd, Z), \+ retired(X) ; (degree(X, ms, Z), \+ retired(X))), subject(Y, Z).


%%%%% Question 4 %%%%%
bit(0).
bit(1).

octal_code(A, B, C) :- bit(A), bit(B), bit(C), format('~w~w~w~n', [A, B, C]).


%%%%% Question 5 %%%%%
color(red).
color(green).
color(blue).

mapcolor(WA, NT, SA, Q, NSW, V, T) :-
    color(WA), color(NT), color(SA), color(Q), color(NSW), color(V), color(T),
    WA \= NT, WA \= SA,
    NT \= SA, NT \= Q,
    SA \= Q, SA \= NSW,
    Q \= NSW, Q \= V, Q \= T,
    NSW \= V,
    V \= T.
