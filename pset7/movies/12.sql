SELECT title FROM movies 
WHERE id IN 
(SELECT movie_id FROM stars WHERE person_id in 
(SELECT id FROM people WHERE name ='Johnny Depp')
INTERSECT
SELECT movie_id FROM stars WHERE person_id in 
(SELECT id FROM people WHERE name ='Helena Bonham Carter'));
