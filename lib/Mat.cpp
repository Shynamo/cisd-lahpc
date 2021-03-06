#include "Mat.h"

#include "util.h"

#include <cstring>
#include <iostream>
#include <new>
#include <random>

namespace my_lapack {

    Mat::~Mat()
    {
        if ( storage != nullptr ) delete[] storage;
    }

    Mat::Mat() { storage = nullptr; }

    Mat::Mat( int m, int n )
        : m( m )
        , n( n )
    {
        storage = initStorage( m * n );
    }

    Mat::Mat( int m, int n, double value )
        : m( m )
        , n( n )
    {
        storage = initStorage( m * n );
        if ( false && value == 0.0 ) {
            memset( storage, 0, static_cast<std::size_t>( m ) * static_cast<std::size_t>( n ) * sizeof( double ) );
        }
        else {
            for ( int i = 0; i < m * n; ++i ) {
                storage[i] = value;
            }
        }
    }

    Mat::Mat( const Mat &other )
        : m( other.m )
        , n( other.n )
    {
        storage = initStorage( m * n );
        memcpy( storage,
                other.storage,
                static_cast<std::size_t>( m ) * static_cast<std::size_t>( n ) * sizeof( double ) );
    }

    double *Mat::col( int j ) { return storage + static_cast<std::size_t>( j ) * static_cast<std::size_t>( m ); }

    void Mat::reshape( int m, int n, double value )
    {
        if ( m * n != this->m * this->n ) {
            std::cerr << "Dims products are different, cannot reshape" << std::endl;
            return;
        }
        this->m = m;
        this->n = n;
        for ( int i = 0; i < m * n; ++i ) {
            storage[i] = value;
        }
    }

    Mat &Mat::operator=( const Mat &other )
    {
        if ( &other != this ) {
            // I chose to offer the strong exception safety.
            // Though it eats up memory...
            double *tmp = initStorage( other.m * other.n );
            for ( int i = 0; i < other.m * other.n; ++i ) {
                tmp[i] = other.storage[i];
            }

            if ( storage != nullptr ) delete[] storage;
            storage = tmp;
            m       = other.m;
            n       = other.n;
        }

        return *this;
    }

    bool Mat::operator==( const Mat &other )
    {
        return m == other.m && n == other.n &&
               ( memcmp( storage,
                         other.storage,
                         static_cast<std::size_t>( m ) * static_cast<std::size_t>( n ) * sizeof( double ) ) == 0 );
    }

    double *Mat::initStorage( int size )
    {
        try {
            return new double[size];
        }
        catch ( const std::bad_alloc &e ) {
            std::cerr << "ERROR::Mat::Mat()\n" << e.what() << std::endl;
            throw e;
        }
    }

    void Mat::print( int precision /*= 6*/ )
    {
        int     m = dimX();
        int     n = dimY();
        double *a = storage;

        auto oldPrecision = std::cout.precision( precision );

        for ( int i = 0; i < m; ++i ) {
            std::cout << "( " << i << " )| ";
            for ( int j = 0; j < n; ++j ) {
                std::cout << a[j * m + i] << " ";
            }
            std::cout << "|\n";
        }

        std::cout << std::endl;
        std::cout.precision( oldPrecision );
    }

    void Mat::fill( double d )
    {
        int len = m * n;
        for ( int i = 0; i < len; i++ ) {
            storage[i] = d;
        }
    }

    std::minstd_rand &GetRandEngine()
    {
        static std::minstd_rand randEngine;
        return randEngine;
    }

    my_lapack::Mat MatSequenceRow( int m, int n )
    {
        Mat mat( m, n );
        int k = 0;
        for ( int j = 0; j < n; ++j ) {
            for ( int i = 0; i < m; ++i ) {
                mat.at( i, j ) = k++;
            }
        }

        return mat;
    }

    Mat MatRandi( int m, int n, unsigned int max, unsigned int seed /*= 0x9d2c5680*/ )
    {
        auto &randEngine = GetRandEngine();
        randEngine.seed( seed );

        Mat mat( m, n );
        for ( int j = 0; j < n; ++j ) {
            for ( int i = 0; i < m; ++i ) {
                mat.at( i, j ) = randEngine() % ( max + 1 );
            }
        }

        return mat;
    }

    Mat MatSqrDiag( int m, double v )
    {
        Mat mat( m, m, 0 );
        for ( int i = 0; i < m; ++i ) {
            mat.at( i, i ) = v;
        }

        return mat;
    }

    Mat MatZero( int m, int n ) { return Mat( m, n, 0.0 ); }

    Mat MatRandUi( int m )
    {
        auto &randEngine = GetRandEngine();

        Mat mat( m, m, 0.0 );
        for ( int j = 0; j < m; ++j ) {
            for ( int i = 0; i <= j; ++i ) {
                mat.at( i, j ) = 1u + ( randEngine() % 128u );
            }
        }
        return mat;
    }

    Mat MatRandLi( int m )
    {
        auto &randEngine = GetRandEngine();

        Mat mat( m, m, 0.0 );
        for ( int i = 0; i < m; ++i ) {
            mat.at( i, i ) = 1.0;
            for ( int j = 0; j < i; ++j ) {
                mat.at( i, j ) = 1u + ( randEngine() % 128u );
            }
        }
        return mat;
    }

    bool Mat::equals( const Mat &mat, double epsilon )
    {
        if ( ( m != mat.m ) || ( n != mat.n ) ) { return 0; }

        int len = m * n;
        for ( int i = 0; i < len; i++ ) {
            if ( !dequals( storage[i], mat.at( i ), epsilon ) ) { return false; }
        }
        return true;
    }

    bool Mat::containsOnly( const double d )
    {
        int len = m * n;
        for ( int i = 0; i < len; i++ ) {
            if ( !dequals( storage[i], d, 2 * LAHPC_EPSILON ) ) { return false; }
        }
        return true;
    }

} // namespace my_lapack
