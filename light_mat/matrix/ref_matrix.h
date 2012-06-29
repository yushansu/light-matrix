/**
 * @file ref_matrix.h
 *
 * ref_matrix classes
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHTMAT_REF_MATRIX_H_
#define LIGHTMAT_REF_MATRIX_H_

#include "bits/ref_matrix_internal.h"

namespace lmat
{
	/********************************************
	 *
	 *  cref_matrix
	 *
	 ********************************************/

	template<typename T, int CTRows, int CTCols, typename Align>
	struct matrix_traits<cref_matrix<T, CTRows, CTCols, Align> >
	{
		static const int num_dimensions = 2;
		static const int compile_time_num_rows = CTRows;
		static const int compile_time_num_cols = CTCols;

		static const bool is_readonly = true;

		typedef T value_type;
	};

	template<typename T, int CTRows, int CTCols, typename Align>
	struct has_continuous_layout<cref_matrix<T, CTRows, CTCols, Align> >
	{
		static const bool value = true;
	};

	template<typename T, int CTRows, int CTCols, typename Align>
	struct is_base_aligned<cref_matrix<T, CTRows, CTCols, Align> >
	{
		static const bool value = is_base_aligned_from_tag<Align>::value;
	};

	template<typename T, int CTRows, int CTCols, typename Align>
	struct is_percol_aligned<cref_matrix<T, CTRows, CTCols, Align> >
	{
		static const bool value = is_percol_aligned_from_tag<Align>::value;
	};


	template<typename T, int CTRows, int CTCols>
	struct is_linear_accessible<cref_matrix<T, CTRows, CTCols> >
	{
		static const bool value = true;
	};


	template<typename T, int CTRows, int CTCols, typename Align>
	class cref_matrix : public IDenseMatrix<cref_matrix<T, CTRows, CTCols, Align>, T>
	{
	public:
		LMAT_MAT_TRAITS_CDEFS(T)

	private:
		static const int CTSize = CTRows * CTCols;
		static const bool IsDynamic = (CTSize == 0);

	public:

		LMAT_ENSURE_INLINE
		cref_matrix(const T* pdata, index_type m, index_type n)
		: m_internal(pdata, m, n)
		{
		}

	private:
		cref_matrix& operator = (const cref_matrix& );  // no assignment

	public:
		LMAT_ENSURE_INLINE index_type nelems() const
		{
			return m_internal.nelems();
		}

		LMAT_ENSURE_INLINE size_type size() const
		{
			return static_cast<size_type>(nelems());
		}

		LMAT_ENSURE_INLINE index_type nrows() const
		{
			return m_internal.nrows();
		}

		LMAT_ENSURE_INLINE index_type ncolumns() const
		{
			return m_internal.ncolumns();
		}

		LMAT_ENSURE_INLINE index_type lead_dim() const
		{
			return m_internal.lead_dim();
		}

		LMAT_ENSURE_INLINE const_pointer ptr_data() const
		{
			return m_internal.ptr_data();
		}

		LMAT_ENSURE_INLINE const_pointer ptr_col(const index_type j) const
		{
			return ptr_data() + j * lead_dim();
		}

		LMAT_ENSURE_INLINE index_type offset(const index_type i, const index_type j) const
		{
			return matrix_indexer<CTRows, CTCols>::offset_c(lead_dim(), i, j);
		}

		LMAT_ENSURE_INLINE const_reference elem(const index_type i, const index_type j) const
		{
			return m_internal.ptr_data()[offset(i, j)];
		}

		LMAT_ENSURE_INLINE const_reference operator[] (const index_type i) const
		{
			return m_internal.ptr_data()[i];
		}

	private:
		detail::ref_matrix_internal<const T, CTRows, CTCols> m_internal;

	}; // end class cref_matrix


	/********************************************
	 *
	 *  ref_matrix
	 *
	 ********************************************/


	template<typename T, int CTRows, int CTCols, typename Align>
	struct matrix_traits<ref_matrix<T, CTRows, CTCols, Align> >
	{
		static const int num_dimensions = 2;
		static const int compile_time_num_rows = CTRows;
		static const int compile_time_num_cols = CTCols;

		static const bool is_readonly = false;

		typedef T value_type;
	};

	template<typename T, int CTRows, int CTCols, typename Align>
	struct has_continuous_layout<ref_matrix<T, CTRows, CTCols, Align> >
	{
		static const bool value = true;
	};

	template<typename T, int CTRows, int CTCols, typename Align>
	struct is_base_aligned<ref_matrix<T, CTRows, CTCols, Align> >
	{
		static const bool value = is_base_aligned_from_tag<Align>::value;
	};

	template<typename T, int CTRows, int CTCols, typename Align>
	struct is_percol_aligned<ref_matrix<T, CTRows, CTCols, Align> >
	{
		static const bool value = is_percol_aligned_from_tag<Align>::value;
	};

	template<typename T, int CTRows, int CTCols>
	struct is_linear_accessible<ref_matrix<T, CTRows, CTCols> >
	{
		static const bool value = true;
	};


	template<typename T, int CTRows, int CTCols, typename Align>
	class ref_matrix : public IDenseMatrix<ref_matrix<T, CTRows, CTCols, Align>, T>
	{
	public:
		LMAT_MAT_TRAITS_DEFS(T)

	public:
		LMAT_ENSURE_INLINE
		ref_matrix(T* pdata, index_type m, index_type n)
		: m_internal(pdata, m, n)
		{
		}

	public:
		LMAT_ENSURE_INLINE ref_matrix& operator = (const ref_matrix& r)
		{
			if (this != &r)
			{
				copy_from_mat(r);
			}
			return *this;
		}

		template<class Mat>
		LMAT_ENSURE_INLINE ref_matrix& operator = (const IDenseMatrix<Other, T>& r)
		{
			copy_from_mat(r);
			return *this;
		}

		template<class Expr>
		LMAT_ENSURE_INLINE ref_matrix& operator = (const IMatrixXpr<Expr, T>& r)
		{
			evaluate_to(r, *this);
			return *this;
		}

		template<class Gen>
		LMAT_ENSURE_INLINE ref_matrix& operator = (const IMatrixGenerator<Gen, T>& gen)
		{
			gen.generate_to(nrows(), ncolumns(), lead_dim(), ptr_data());
			return *this;
		}

	public:
		LMAT_ENSURE_INLINE index_type nelems() const
		{
			return m_internal.nelems();
		}

		LMAT_ENSURE_INLINE size_type size() const
		{
			return static_cast<size_type>(nelems());
		}

		LMAT_ENSURE_INLINE index_type nrows() const
		{
			return m_internal.nrows();
		}

		LMAT_ENSURE_INLINE index_type ncolumns() const
		{
			return m_internal.ncolumns();
		}

		LMAT_ENSURE_INLINE index_type lead_dim() const
		{
			return m_internal.lead_dim();
		}

		LMAT_ENSURE_INLINE const_pointer ptr_data() const
		{
			return m_internal.ptr_data();
		}

		LMAT_ENSURE_INLINE pointer ptr_data()
		{
			return m_internal.ptr_data();
		}

		LMAT_ENSURE_INLINE const_pointer ptr_col(const index_type j) const
		{
			return ptr_data() + j * lead_dim();
		}

		LMAT_ENSURE_INLINE pointer ptr_col(const index_type j)
		{
			return ptr_data() + j * lead_dim();
		}

		LMAT_ENSURE_INLINE index_type offset(const index_type i, const index_type j) const
		{
			return matrix_indexer<CTRows, CTCols>::offset_c(lead_dim(), i, j);
		}

		LMAT_ENSURE_INLINE const_reference elem(const index_type i, const index_type j) const
		{
			return m_internal.ptr_data()[offset(i, j)];
		}

		LMAT_ENSURE_INLINE reference elem(const index_type i, const index_type j)
		{
			return m_internal.ptr_data()[offset(i, j)];
		}

		LMAT_ENSURE_INLINE const_reference operator[] (const index_type i) const
		{
			return m_internal.ptr_data()[i];
		}

		LMAT_ENSURE_INLINE reference operator[] (const index_type i)
		{
			return m_internal.ptr_data()[i];
		}

	private:
		template<class Mat>
		LMAT_ENSURE_INLINE
		void copy_from_mat(const IDenseMatrix<Mat, T>& r)
		{
			if (has_continuous_layout<Mat>::value)
			{
				if ( !(ptr_data() == r.ptr_data()) )
				{
					copy_mem(nelems(), r.ptr_data(), ptr_data());
				}
			}
			else
			{
				if ( !(ptr_data() == r.ptr_data() && lead_dim() == r.lead_dim()) )
				{
					copy(r.derived(), *this);
				}
			}
		}

	private:
		detail::ref_matrix_internal<T, CTRows, CTCols> m_internal;

	}; // end ref_matrix


	/********************************************
	 *
	 *  vectors derived from (c)ref_matrix
	 *
	 ********************************************/

	template<typename T, int CTRows>
	class cref_col: public cref_matrix<T, CTRows, 1>
	{
		typedef cref_matrix<T, CTRows, 1> base_mat_t;

	public:
		typedef index_t index_type;

		LMAT_ENSURE_INLINE
		cref_col(const T* pdata, index_type m)
		: base_mat_t(pdata, m, 1) { }
	};

	template<typename T, int CTRows>
	class ref_col: public ref_matrix<T, CTRows, 1>
	{
		typedef ref_matrix<T, CTRows, 1> base_mat_t;

	public:
		typedef index_t index_type;

		LMAT_ENSURE_INLINE
		ref_col(T* pdata, index_type m)
		: base_mat_t(pdata, m, 1) { }

		LMAT_ENSURE_INLINE ref_col& operator = (const base_mat_t& r)
		{
			base_mat_t::operator = (r);
			return *this;
		}

		template<class Mat>
		LMAT_ENSURE_INLINE ref_col& operator = (const IDenseMatrix<Mat, T>& r)
		{
			base_mat_t::operator = (r.derived());
			return *this;
		}

		template<class Expr>
		LMAT_ENSURE_INLINE ref_col& operator = (const IMatrixXpr<Expr, T>& r)
		{
			base_mat_t::operator = (r.derived());
			return *this;
		}

		template<class Gen>
		LMAT_ENSURE_INLINE ref_col& operator = (const IMatrixGenerator<Gen, T>& gen)
		{
			base_mat_t::operator = (gen.derived());
			return *this;
		}
	};


	template<typename T, int CTCols>
	class cref_row: public cref_matrix<T, 1, CTCols>
	{
		typedef cref_matrix<T, 1, CTCols> base_mat_t;

	public:
		typedef index_t index_type;

		LMAT_ENSURE_INLINE
		cref_row(const T* pdata, index_type n)
		: base_mat_t(pdata, 1, n) { }
	};

	template<typename T, int CTCols>
	class ref_row: public ref_matrix<T, 1, CTCols>
	{
		typedef ref_matrix<T, 1, CTCols> base_mat_t;

	public:
		typedef index_t index_type;

		LMAT_ENSURE_INLINE
		ref_row(T* pdata, index_type n)
		: base_mat_t(pdata, 1, n) { }

		LMAT_ENSURE_INLINE ref_row& operator = (const base_mat_t& r)
		{
			base_mat_t::operator = (r);
			return *this;
		}

		template<class Mat>
		LMAT_ENSURE_INLINE ref_row& operator = (const IDenseMatrix<Mat, T>& r)
		{
			base_mat_t::operator = (r.derived());
			return *this;
		}

		template<class Expr>
		LMAT_ENSURE_INLINE ref_row& operator = (const IMatrixXpr<Expr, T>& r)
		{
			base_mat_t::operator = (r.derived());
			return *this;
		}

		template<class Gen>
		LMAT_ENSURE_INLINE ref_row& operator = (const IMatrixGenerator<Gen, T>& gen)
		{
			base_mat_t::operator = (gen.derived());
			return *this;
		}
	};

}

#endif /* REF_MATRIX_H_ */