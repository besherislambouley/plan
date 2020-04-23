/*
 * let dis[i] is the distance between node i and the nearest NPP node , use multisource dijkstra
 * loop through the nodes in decreasing order according to dis[node] , make yes[node] = 1 & merge it with the neighboring nodes u  with yes[u]=1 also 
 * we will try to build MST so we will just merge n-1 times with edges value the minmum between dis[u] , dis[v] 
 * build sparse table on the last tree , the answer to the query is the smallest value of a edge on the path from s to t 
*/
#define fast ios_base::sync_with_stdio(0),cin.tie(0),cout.tie(0)
#include <bits/stdc++.h>
using namespace std;
#define sqr 547
#define mid (l+r)/2
#define pb push_back
#define ppb pop_back
#define fi first
#define se second
#define lb lower_bound
#define ub upper_bound
#define ins insert
#define era erase
#define C continue
#define mem(dp,i) memset(dp,i,sizeof(dp))
#define mset multiset
#define all(x) x.begin(), x.end()
typedef long long ll;
typedef short int si;
typedef long double ld;
typedef pair<int,int> pi;
typedef pair<ll,ll> pll;
typedef vector<int> vi;
typedef vector<ll> vll;
typedef vector<pi> vpi;
typedef vector<pll> vpll;
const ll inf=1e18;
const ll mod=1e9+7;
const ld pai=acos(-1);
int n , m , k , q ;
vpi v[100009] , g[100009] ;
int a[100009] , p [100009] , ord [100009] , yes [100009] ;
// dijkstra 
set < pi > s ;
int dis [100009] , done [100009] ;
bool cmp ( int i , int j ) {
        return dis [i] > dis [j] ;
}
void dijkstra () {
	for ( int i = 0 ; i < n ; i ++ ) dis [i] = 1e9 ;
	for ( int i = 0 ; i < k ; i ++ ) {
		dis [ a[i] ] = 0 ;
		s .ins ( { 0 , a[i] } ) ;
	}
	while ( s.size() ) {
		int node = s.begin()->se ;
		int t = s.begin()->fi ;
		s.era ( s.begin() ) ;
		done [ node ] = 1 ;
		for ( auto U : v[node] ) {
			int u = U.fi ;
			int x = U.se ;
			if ( done [ u ] ) C ;
			if ( s.count ( { dis[u] , u } ) ) s.era ( { dis[u] , u } ) ;
			dis [u] = min ( dis[u] , t + x ) ;
			s.ins ( { dis[u] , u } ) ;
		}
	}
}
// dsu 
int get ( int node ) {
	if ( node == p [node] ) return node ;
	return p [node] = get ( p [ node ] ) ;
}
void merge ( int a , int b , int c ) {
	int A = get ( a ) ;
	int B = get ( b ) ;
	if ( A == B ) return ;
	g [a] .pb ( { b , c } ) ;
	g [b] .pb ( { a , c } ) ;
	p [B] = A ;
}
//
int dp[100009][20] , mn[100009][20] , w[100009] ;
void dfs ( int node, int p ) {
	dp [node][0] = p ;
	w  [node] = w[p] + 1 ;
	for ( auto u : g[node] ) {
		if ( u .fi == p ) C ;
		mn [u.fi][0] = u.se ;
		dfs ( u.fi , node ) ;
	}
}
void build () {
	for ( int i = 0 ; i < n ; i ++ ) {
		for ( int j = 0 ; j < 20 ; j ++ ) {
			mn [i][j] = 1e9 ;
		}
	}
	dfs ( 0 , 0 ) ;
	for ( int j = 1 ; j < 20 ; j ++ ) {
		for ( int i = 0 ; i < n ; i ++ ) {
			dp [i][j] = dp[ dp[i][j-1] ] [j-1] ;
			mn [i][j] = min ( mn[i][j-1] , mn [ dp[i][j-1] ] [j-1] ) ;
		}
	}
}
int lca ( int a , int b ) {
	int ans = 1e9 ; 
	if ( w[a] < w[b] ) swap ( a , b ) ;
	int l = w[a] - w[b] ;
	for ( int i = 0 ; i < 20 ; i ++ ) {
		if ( ( l & ( 1<<i ) ) ) {
			ans = min ( ans , mn[a][i] ) ;
			a = dp[a][i] ;
		}
	}
	if ( a == b ) return ans ;
	for ( int i = 19 ; i >= 0 ; i -- ) {
		if ( dp[a][i] != dp[b][i] ) {
			ans = min ( ans , min ( mn[a][i] , mn[b][i] ) ) ;
			a = dp[a][i] ;
			b = dp[b][i] ; 
		}
	}
	ans = min ( ans , min ( mn[a][0] , mn[b][0] ) ) ;
	return ans ;
}
int main () {
	cin >> n >> m ;
	for ( int i = 0 ; i < n ; i ++ ) p[i] = i , ord[i] = i ;
	for ( int i = 0 ; i < m ; i ++ ) {
		int a , b , c ;
		cin >> a >> b >> c ;
		a -- , b -- ;
		v[a] .pb ( { b , c } ) ;
		v[b] .pb ( { a , c } ) ;	
	}
	cin >> k ;
	for ( int i = 0 ; i < k ; i ++ ) cin >> a[i] , a[i] -- ;
	dijkstra () ;
	sort ( ord , ord + n , cmp ) ;
	for ( int i = 0 ; i < n ; i ++ ) {
		int node = ord [i] ; 
		yes [ node ] = 1 ;
		for ( auto U : v[node] ) {
			int u = U .fi ;
			int x = U .se ;
			if ( yes [u] ) {
				merge ( u , node , min ( dis[u] , dis[node] ) ) ;
			}
		}
	}
	build () ;
	cin >> q ;
	while ( q -- ) {
		int s , t ;
		cin >> s >> t ;
		s -- , t -- ;
		cout << lca ( s , t ) << endl ; 
	}
}
