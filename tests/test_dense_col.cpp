/**
 * @file test_dense_col.cpp
 *
 * Unit testing of dense_col
 *
 * @author Dahua Lin
 */


#include "test_base.h"

#include <light_mat/matrix/dense_matrix.h>

using namespace lmat;
using namespace lmat::test;

// explicit instantiation

template class lmat::dense_col<double, DynamicDim>;
template class lmat::dense_col<double, 4>;

#ifdef LMAT_USE_STATIC_ASSERT
static_assert(sizeof(lmat::col2_f64) == sizeof(double) * 2, "Incorrect size for fixed-size matrices");
static_assert(sizeof(lmat::col3_f64) == sizeof(double) * 3, "Incorrect size for fixed-size matrices");

static_assert(lmat::is_base_of<
		lmat::dense_matrix<double, DynamicDim, 1>,
		lmat::dense_col<double, DynamicDim> >::value, "Base verification failed.");

static_assert(lmat::is_base_of<
		lmat::dense_matrix<double, 4, 1>,
		lmat::dense_col<double, 4> >::value, "Base verification failed.");

#endif


N_CASE( dense_col, constructs )
{
	// default construction

	dense_col<double, N> a0;

	ASSERT_EQ(a0.nrows(), N);
	ASSERT_EQ(a0.ncolumns(), 1);
	ASSERT_EQ(a0.nelems(), N);
	ASSERT_EQ(a0.lead_dim(), N);

	ASSERT_EQ(a0.size(), (size_t)a0.nelems() );

	if (N > 0)
	{
		ASSERT_TRUE(a0.ptr_data() != 0);
	}
	else
	{
		ASSERT_TRUE(a0.ptr_data() == 0);
	}

	// size given construction

	const index_t n = N == 0 ? 4 : N;

	dense_col<double, N> a1(n);

	ASSERT_EQ(a1.nrows(), n);
	ASSERT_EQ(a1.ncolumns(), 1);
	ASSERT_EQ(a1.nelems(), n);
	ASSERT_EQ(a1.lead_dim(), n);

	ASSERT_EQ(a1.size(), (size_t)a1.nelems() );
	ASSERT_TRUE(a1.ptr_data() != 0);
}


N_CASE( dense_col, generates )
{
	const index_t n = N == 0 ? 4 : N;

	scoped_array<double> ref(n);

	// zeros

	dense_col<double, N> a0(n, zeros<double>());
	for (index_t i = 0; i < n; ++i) ref[i] = double(0);

	ASSERT_EQ(a0.nrows(), n);
	ASSERT_EQ(a0.ncolumns(), 1);
	ASSERT_EQ(a0.nelems(), n);
	ASSERT_EQ(a0.lead_dim(), n);
	ASSERT_VEC_EQ(n, a0, ref);

	// fill_value

	const double v1 = 2.5;
	dense_col<double, N> a1(n, fill_value(v1));
	for (index_t i = 0; i < n; ++i) ref[i] = v1;

	ASSERT_EQ(a1.nrows(), n);
	ASSERT_EQ(a1.ncolumns(), 1);
	ASSERT_EQ(a1.nelems(), n);
	ASSERT_EQ(a1.lead_dim(), n);
	ASSERT_VEC_EQ(n, a1, ref);

	// copy_value

	for (index_t i = 0; i < n; ++i) ref[i] = double(i + 2);
	dense_col<double, N> a2(n, copy_from(ref.ptr_begin()));

	ASSERT_EQ(a2.nrows(), n);
	ASSERT_EQ(a2.ncolumns(), 1);
	ASSERT_EQ(a2.nelems(), n);
	ASSERT_EQ(a2.lead_dim(), n);
	ASSERT_VEC_EQ(n, a2, ref);
}

N_CASE( dense_col, copy_constructs )
{
	const index_t n = N == 0 ? 4 : N;

	scoped_array<double> ref(n);

	for (index_t i = 0; i < n; ++i) ref[i] = double(i + 2);
	dense_col<double, N> a(n, copy_from(ref.ptr_begin()));

	dense_col<double, N> a2(a);

	ASSERT_EQ(a2.nrows(), n);
	ASSERT_EQ(a2.ncolumns(), 1);
	ASSERT_EQ(a2.nelems(), n);
	ASSERT_EQ(a2.lead_dim(), n);

	ASSERT_NE(a.ptr_data(), a2.ptr_data());

	ASSERT_VEC_EQ(n, a, a2);
}


N_CASE( dense_col, resize )
{
	const index_t n = N == 0 ? 4 : N;
	const index_t n2 = N == 0 ? 5 : N;

	dense_col<double, N> a(n);
	const double *p1 = a.ptr_data();

	a.resize(n, 1);

	ASSERT_EQ(a.nrows(), n);
	ASSERT_EQ(a.ncolumns(), 1);
	ASSERT_EQ(a.nelems(), n);
	ASSERT_EQ(a.lead_dim(), n);

	ASSERT_EQ(a.ptr_data(), p1);

	a.resize(n);

	ASSERT_EQ(a.nrows(), n);
	ASSERT_EQ(a.ncolumns(), 1);
	ASSERT_EQ(a.nelems(), n);
	ASSERT_EQ(a.lead_dim(), n);

	ASSERT_EQ(a.ptr_data(), p1);

	a.resize(n2);

	ASSERT_EQ(a.nrows(), n2);
	ASSERT_EQ(a.ncolumns(), 1);
	ASSERT_EQ(a.nelems(), n2);
	ASSERT_EQ(a.lead_dim(), n2);

	const double *p2 = a.ptr_data();

	if (n2 == n)
	{
		ASSERT_EQ( p2, p1 );
	}
	else
	{
		ASSERT_NE( p2, p1 );
	}
}


N_CASE( dense_col, assign )
{
	const index_t n = N == 0 ? 4 : N;

	scoped_array<double> ref(n);

	for (index_t i = 0; i < n; ++i) ref[i] = double(i + 2);
	dense_col<double, N> s(n, copy_from(ref.ptr_begin()));

	dense_col<double, N> a;

	a = s;

	ASSERT_NE( a.ptr_data(), 0 );
	ASSERT_NE( a.ptr_data(), s.ptr_data() );
	ASSERT_EQ( a.nrows(), n);
	ASSERT_EQ( a.ncolumns(), 1);
	ASSERT_EQ( a.nelems(), n);
	ASSERT_EQ( a.lead_dim(), n);

	ASSERT_VEC_EQ( n, a, s );

	dense_col<double, N> b(n, zeros<double>());

	const double *pb = b.ptr_data();

	ASSERT_NE( pb, 0 );
	ASSERT_NE( pb, s.ptr_data() );

	b = s;

	ASSERT_EQ( b.ptr_data(), pb );
	ASSERT_EQ( b.nrows(), n);
	ASSERT_EQ( b.ncolumns(), 1);
	ASSERT_EQ( b.nelems(), n);
	ASSERT_EQ( b.lead_dim(), n);

	ASSERT_VEC_EQ( n, b, s );

	const index_t n2 = N == 0 ? 6 : N;

	dense_col<double, N> c(n2, zeros<double>());

	c = s;

	ASSERT_NE( c.ptr_data(), 0 );
	ASSERT_NE( c.ptr_data(), s.ptr_data() );
	ASSERT_EQ( c.nrows(), n);
	ASSERT_EQ( c.ncolumns(), 1);
	ASSERT_EQ( c.nelems(), n);
	ASSERT_EQ( c.lead_dim(), n);

	ASSERT_VEC_EQ( n, c, s );
}


N_CASE( dense_col, assign_gen )
{
	const index_t n = N == 0 ? 4 : N;

	scoped_array<double> ref(n);

	// zeros

	dense_col<double, N> a(n, fill_value(-1.0));

	a = zeros<double>();
	for (index_t i = 0; i < n; ++i) ref[i] = double(0);

	ASSERT_EQ(a.nrows(), n);
	ASSERT_EQ(a.ncolumns(), 1);
	ASSERT_EQ(a.nelems(), n);
	ASSERT_EQ(a.lead_dim(), n);
	ASSERT_VEC_EQ(n, a, ref);

	// fill_value

	const double v1 = 2.5;
	a = fill_value(v1);
	for (index_t i = 0; i < n; ++i) ref[i] = v1;

	ASSERT_EQ(a.nrows(), n);
	ASSERT_EQ(a.ncolumns(), 1);
	ASSERT_EQ(a.nelems(), n);
	ASSERT_EQ(a.lead_dim(), n);
	ASSERT_VEC_EQ(n, a, ref);

	// copy_value

	for (index_t i = 0; i < n; ++i) ref[i] = double(i + 2);
	a = copy_from(ref.ptr_begin());

	ASSERT_EQ(a.nrows(), n);
	ASSERT_EQ(a.ncolumns(), 1);
	ASSERT_EQ(a.nelems(), n);
	ASSERT_EQ(a.lead_dim(), n);
	ASSERT_VEC_EQ(n, a, ref);
}

N_CASE( dense_col, swap )
{
	const index_t n = N == 0 ? 4 : N;
	const index_t n2 = N == 0 ? 5 : N;

	scoped_array<double> s(n);
	for (index_t i = 0; i < n; ++i) s[i] = double(i + 2);

	scoped_array<double> s2(n2);
	for (index_t i = 0; i < n2; ++i) s2[i] = double(2 * i + 3);

	dense_col<double, N> a(n, copy_from(s.ptr_begin()));
	dense_col<double, N> a2(n2, copy_from(s2.ptr_begin()));

	const double *p = a.ptr_data();
	const double *p2 = a2.ptr_data();

	swap(a, a2);

	ASSERT_EQ( a.nrows(), n2 );
	ASSERT_EQ( a.ncolumns(), 1 );
	ASSERT_EQ( a.nelems(), n2 );

	ASSERT_EQ( a2.nrows(), n );
	ASSERT_EQ( a2.ncolumns(), 1 );
	ASSERT_EQ( a2.nelems(), n );

	if (N == 0)
	{
		ASSERT_EQ( a.ptr_data(), p2 );
		ASSERT_EQ( a2.ptr_data(), p );
	}
	else
	{
		ASSERT_EQ( a.ptr_data(), p );
		ASSERT_EQ( a2.ptr_data(), p2 );
	}

	ASSERT_VEC_EQ( n2, a, s2 );
	ASSERT_VEC_EQ( n, a2, s );
}




BEGIN_TPACK( dense_col_constructs )
	ADD_N_CASE( dense_col, constructs, 0 )
	ADD_N_CASE( dense_col, constructs, 1 )
	ADD_N_CASE( dense_col, constructs, 4 )
END_TPACK

BEGIN_TPACK( dense_col_generates )
	ADD_N_CASE( dense_col, generates, 0 )
	ADD_N_CASE( dense_col, generates, 1 )
	ADD_N_CASE( dense_col, generates, 4 )
END_TPACK

BEGIN_TPACK( dense_col_copycon )
	ADD_N_CASE( dense_col, copy_constructs, 0 )
	ADD_N_CASE( dense_col, copy_constructs, 1 )
	ADD_N_CASE( dense_col, copy_constructs, 4 )
END_TPACK

BEGIN_TPACK( dense_col_resize )
	ADD_N_CASE( dense_col, resize, 0 )
	ADD_N_CASE( dense_col, resize, 1 )
	ADD_N_CASE( dense_col, resize, 4 )
END_TPACK

BEGIN_TPACK( dense_col_assign )
	ADD_N_CASE( dense_col, assign, 0 )
	ADD_N_CASE( dense_col, assign, 1 )
	ADD_N_CASE( dense_col, assign, 4 )
END_TPACK

BEGIN_TPACK( dense_col_assign_gen )
	ADD_N_CASE( dense_col, assign_gen, 0 )
	ADD_N_CASE( dense_col, assign_gen, 1 )
	ADD_N_CASE( dense_col, assign_gen, 4 )
END_TPACK

BEGIN_TPACK( dense_col_swap )
	ADD_N_CASE( dense_col, swap, 0 )
	ADD_N_CASE( dense_col, swap, 1 )
	ADD_N_CASE( dense_col, swap, 4 )
END_TPACK

BEGIN_MAIN_SUITE
	ADD_TPACK( dense_col_constructs )
	ADD_TPACK( dense_col_generates )
	ADD_TPACK( dense_col_copycon )
	ADD_TPACK( dense_col_resize )
	ADD_TPACK( dense_col_assign )
	ADD_TPACK( dense_col_assign_gen )
	ADD_TPACK( dense_col_swap )
END_MAIN_SUITE


