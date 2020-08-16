SELECT title FROM movies  JOIN ratings on ratings.movie_id = movies.id WHERE id in (SELECT movie_id FROM stars where person_id in (SELECT id FROM people WHERE name = "Chadwick Boseman")) ORDER BY ratings.rating DESC LIMIT 5;